#pragma once

#include <string>
#include <cstdint>

class ComputeShader
{
public:
    ComputeShader() = default;
    ~ComputeShader();

    // Prevent copying
    ComputeShader(const ComputeShader&) = delete;
    ComputeShader& operator=(const ComputeShader&) = delete;

    bool Load(const std::string& sourcePath);
    void Unload();

    void Bind() const;
    void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) const;

    void SetUniformFloat(const std::string& name, float value) const;
    void SetUniformInt(const std::string& name, int value) const;
    void SetUniformVec4(const std::string& name, float x, float y, float z, float w) const;

    unsigned int GetProgramId() const { return programId; }

private:
    unsigned int programId = 0;
};
