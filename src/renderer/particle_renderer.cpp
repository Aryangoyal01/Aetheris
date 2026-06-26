#include "particle_renderer.h"
#include "gpu_allocator.h"
#include "gl_helpers.h"
#include "rlgl.h"
#include "raymath.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

ParticleRenderer::ParticleRenderer() = default;
ParticleRenderer::~ParticleRenderer() { Shutdown(); }

static std::string LoadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static unsigned int CompileShader(const char* source, unsigned int type) {
    unsigned int shader = ext_glCreateShader ? ext_glCreateShader(type) : 0;
    if (!shader) return 0;
    ext_glShaderSource(shader, 1, &source, nullptr);
    ext_glCompileShader(shader);
    int ok = 0;
    ext_glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512] = {};
        ext_glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        ext_glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool ParticleRenderer::LoadRenderShader() {
    std::string vertSrc = LoadFile("src/shaders/particle_render.vert");
    std::string fragSrc = LoadFile("src/shaders/particle_render.frag");
    if (vertSrc.empty() || fragSrc.empty()) return false;

    unsigned int vert = CompileShader(vertSrc.c_str(), GL_VERTEX_SHADER);
    unsigned int frag = CompileShader(fragSrc.c_str(), GL_FRAGMENT_SHADER);
    if (!vert || !frag) {
        if (vert) ext_glDeleteShader(vert);
        if (frag) ext_glDeleteShader(frag);
        return false;
    }

    m_renderProgram = ext_glCreateProgram();
    ext_glAttachShader(m_renderProgram, vert);
    ext_glAttachShader(m_renderProgram, frag);
    ext_glLinkProgram(m_renderProgram);

    int linked = 0;
    ext_glGetProgramiv(m_renderProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        ext_glDeleteProgram(m_renderProgram);
        m_renderProgram = 0;
        ext_glDeleteShader(vert);
        ext_glDeleteShader(frag);
        return false;
    }

    ext_glDeleteShader(vert);
    ext_glDeleteShader(frag);

    m_locMVP = ext_glGetUniformLocation(m_renderProgram, "mvp");
    m_locModelView = ext_glGetUniformLocation(m_renderProgram, "modelView");
    m_locNormalMatrix = ext_glGetUniformLocation(m_renderProgram, "normalMatrix");
    m_locViewPos = ext_glGetUniformLocation(m_renderProgram, "viewPos");

    return true;
}

void ParticleRenderer::CreateUnitSphere(int segments) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float radius = 0.5f;

    for (int lat = 0; lat <= segments; ++lat) {
        float theta = lat * (float)M_PI / segments;
        float sinTheta = sinf(theta);
        float cosTheta = cosf(theta);

        for (int lon = 0; lon <= segments; ++lon) {
            float phi = lon * 2.0f * (float)M_PI / segments;
            float sinPhi = sinf(phi);
            float cosPhi = cosf(phi);

            float nx = cosPhi * sinTheta;
            float ny = cosTheta;
            float nz = sinPhi * sinTheta;

            vertices.push_back(nx * radius);
            vertices.push_back(ny * radius);
            vertices.push_back(nz * radius);

            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }

    for (int lat = 0; lat < segments; ++lat) {
        for (int lon = 0; lon < segments; ++lon) {
            int first = lat * (segments + 1) + lon;
            int second = first + segments + 1;

            indices.push_back((unsigned int)first);
            indices.push_back((unsigned int)second);
            indices.push_back((unsigned int)(first + 1));

            indices.push_back((unsigned int)second);
            indices.push_back((unsigned int)(second + 1));
            indices.push_back((unsigned int)(first + 1));
        }
    }

    m_indexCount = (uint32_t)indices.size();

    ext_glGenVertexArrays(1, &m_vao);
    ext_glBindVertexArray(m_vao);

    ext_glGenBuffers(1, &m_vbo);
    ext_glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    ext_glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    ext_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    ext_glEnableVertexAttribArray(0);
    ext_glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    ext_glEnableVertexAttribArray(1);

    ext_glGenBuffers(1, &m_ebo);
    ext_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    ext_glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    ext_glBindVertexArray(0);
}

bool ParticleRenderer::Init(GPUAllocator& allocator) {
    if (m_initialized) Shutdown();

    if (!LoadRenderShader()) return false;

    CreateUnitSphere(16);

    if (m_vao == 0 || m_vbo == 0 || m_ebo == 0) {
        return false;
    }

    m_initialized = true;
    return true;
}

void ParticleRenderer::Shutdown() {
    if (m_renderProgram != 0) { ext_glDeleteProgram(m_renderProgram); m_renderProgram = 0; }
    if (m_ebo != 0) { ext_glDeleteBuffers(1, &m_ebo); m_ebo = 0; }
    if (m_vbo != 0) { ext_glDeleteBuffers(1, &m_vbo); m_vbo = 0; }
    if (m_vao != 0) { ext_glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
    m_initialized = false;
}

void ParticleRenderer::Render(uint32_t activeCount) {
    if (!m_initialized || activeCount == 0 || m_renderProgram == 0) return;

    Matrix proj = rlGetMatrixProjection();
    Matrix view = rlGetMatrixModelview();
    Matrix mv = view;
    Matrix mvp = MatrixMultiply(view, proj);

    float normalMatrix[9] = {
        mv.m0, mv.m1, mv.m2,
        mv.m4, mv.m5, mv.m6,
        mv.m8, mv.m9, mv.m10
    };

    Vector3 camPos = { mv.m12, mv.m13, mv.m14 };

    ext_glUseProgram(m_renderProgram);

    if (m_locMVP >= 0) ext_glUniformMatrix4fv(m_locMVP, 1, GL_FALSE, &mvp.m0);
    if (m_locModelView >= 0) ext_glUniformMatrix4fv(m_locModelView, 1, GL_FALSE, &mv.m0);
    if (m_locNormalMatrix >= 0) ext_glUniformMatrix3fv(m_locNormalMatrix, 1, GL_FALSE, normalMatrix);
    if (m_locViewPos >= 0) ext_glUniform3f(m_locViewPos, camPos.x, camPos.y, camPos.z);

    ext_glBindVertexArray(m_vao);
    ext_glDrawElementsInstanced(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr, activeCount);
    ext_glBindVertexArray(0);

    ext_glUseProgram(0);
}
