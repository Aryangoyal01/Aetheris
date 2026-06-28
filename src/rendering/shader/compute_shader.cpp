#include "compute_shader.h"
#include "shader.h"
#include <raylib.h>
#include <rlgl.h>

ComputeShader::~ComputeShader()
{
    Unload();
}

bool ComputeShader::Load(const std::string& sourcePath)
{
    Unload();
    programId = ShaderProgram::CompileCompute(sourcePath);
    return programId != 0;
}

void ComputeShader::Unload()
{
    if (programId != 0)
    {
        rlUnloadShaderProgram(programId);
        programId = 0;
    }
}

void ComputeShader::Bind() const
{
    if (programId != 0)
    {
        rlEnableShader(programId);
    }
}

void ComputeShader::Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) const
{
    if (programId != 0)
    {
        rlComputeShaderDispatch(groupX, groupY, groupZ);
    }
}

void ComputeShader::SetUniformFloat(const std::string& name, float value) const
{
    if (programId == 0) return;
    int loc = rlGetLocationUniform(programId, name.c_str());
    if (loc != -1) rlSetUniform(loc, &value, RL_SHADER_UNIFORM_FLOAT, 1);
}

void ComputeShader::SetUniformInt(const std::string& name, int value) const
{
    if (programId == 0) return;
    int loc = rlGetLocationUniform(programId, name.c_str());
    if (loc != -1) rlSetUniform(loc, &value, RL_SHADER_UNIFORM_INT, 1);
}

void ComputeShader::SetUniformVec4(const std::string& name, float x, float y, float z, float w) const
{
    if (programId == 0) return;
    int loc = rlGetLocationUniform(programId, name.c_str());
    if (loc != -1)
    {
        float values[4] = { x, y, z, w };
        rlSetUniform(loc, values, RL_SHADER_UNIFORM_VEC4, 1);
    }
}
