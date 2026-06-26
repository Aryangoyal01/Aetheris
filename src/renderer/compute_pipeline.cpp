#include "compute_pipeline.h"
#include "gpu_allocator.h"
#include "settings.h"
#include "rlgl.h"
#include "gl_helpers.h"
#include <fstream>
#include <sstream>

ComputePipeline::ComputePipeline() = default;
ComputePipeline::~ComputePipeline() { Shutdown(); }

static std::string LoadFileContents(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool ComputePipeline::Init(const std::string& shaderPath, GPUAllocator& allocator) {
    if (m_initialized) Shutdown();

    std::string source = LoadFileContents(shaderPath);
    if (source.empty()) return false;

    unsigned int shaderId = rlCompileShader(source.c_str(), RL_COMPUTE_SHADER);
    if (shaderId == 0) return false;

    m_computeProgram = rlLoadComputeShaderProgram(shaderId);
    if (m_computeProgram == 0) return false;

    CacheUniformLocations();

    if (!LoadGLFunctions()) return false;

    if (allocator.IsInitialized()) {
        rlBindShaderBuffer(allocator.GetSSBO(), 0);
    }

    m_initialized = true;
    return true;
}

void ComputePipeline::Shutdown() {
    if (m_computeProgram != 0) {
        rlUnloadShaderProgram(m_computeProgram);
        m_computeProgram = 0;
    }
    m_initialized = false;
}

void ComputePipeline::Dispatch(uint32_t activeCount, float dt, const EngineSettings& settings,
                               float handWorldX, float handWorldY, float handWorldZ,
                               bool handDetected, bool isPinching) {
    if (!m_initialized || activeCount == 0) return;

    if (ext_glUseProgram) {
        ext_glUseProgram(m_computeProgram);
    }

    if (m_locDt >= 0) rlSetUniform(m_locDt, &dt, RL_SHADER_UNIFORM_FLOAT, 1);
    if (m_locGravity >= 0) rlSetUniform(m_locGravity, &settings.gravityStrength, RL_SHADER_UNIFORM_FLOAT, 1);
    if (m_locFriction >= 0) rlSetUniform(m_locFriction, &settings.frictionCoefficient, RL_SHADER_UNIFORM_FLOAT, 1);
    if (m_locElasticity >= 0) rlSetUniform(m_locElasticity, &settings.elasticityValue, RL_SHADER_UNIFORM_FLOAT, 1);

    float handPos[3] = { handWorldX, handWorldY, handWorldZ };
    if (m_locHandPos >= 0) rlSetUniform(m_locHandPos, handPos, RL_SHADER_UNIFORM_VEC3, 1);

    int hd = handDetected ? 1 : 0;
    int ip = isPinching ? 1 : 0;
    int aot = settings.activeObjectType;
    if (m_locHandDetected >= 0) rlSetUniform(m_locHandDetected, &hd, RL_SHADER_UNIFORM_INT, 1);
    if (m_locIsPinching >= 0) rlSetUniform(m_locIsPinching, &ip, RL_SHADER_UNIFORM_INT, 1);
    if (m_locActiveObjectType >= 0) rlSetUniform(m_locActiveObjectType, &aot, RL_SHADER_UNIFORM_INT, 1);

    float iStrength = 15.0f;
    float iRadius = settings.interactionRadius;
    if (m_locInteractionStrength >= 0) rlSetUniform(m_locInteractionStrength, &iStrength, RL_SHADER_UNIFORM_FLOAT, 1);
    if (m_locInteractionRadius >= 0) rlSetUniform(m_locInteractionRadius, &iRadius, RL_SHADER_UNIFORM_FLOAT, 1);

    uint32_t groupCountX = (activeCount + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;
    rlComputeShaderDispatch(groupCountX, 1, 1);

    if (ext_glMemoryBarrier) {
        ext_glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    }
}

void ComputePipeline::CacheUniformLocations() {
    if (m_computeProgram == 0) return;

    m_locDt = rlGetLocationUniform(m_computeProgram, "dt");
    m_locGravity = rlGetLocationUniform(m_computeProgram, "gravityStrength");
    m_locFriction = rlGetLocationUniform(m_computeProgram, "frictionCoefficient");
    m_locElasticity = rlGetLocationUniform(m_computeProgram, "elasticityValue");
    m_locHandPos = rlGetLocationUniform(m_computeProgram, "handWorldPos");
    m_locHandDetected = rlGetLocationUniform(m_computeProgram, "handDetected");
    m_locIsPinching = rlGetLocationUniform(m_computeProgram, "isPinching");
    m_locActiveObjectType = rlGetLocationUniform(m_computeProgram, "activeObjectType");
    m_locInteractionStrength = rlGetLocationUniform(m_computeProgram, "interactionStrength");
    m_locInteractionRadius = rlGetLocationUniform(m_computeProgram, "interactionRadius");
}
