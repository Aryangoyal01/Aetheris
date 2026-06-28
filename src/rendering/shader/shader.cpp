#include "shader.h"
#include "core/log.h"
#include <raylib.h>
#include <rlgl.h>
#include <fstream>
#include <sstream>

std::string ShaderProgram::ReadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::string err = "Failed to open shader file: " + path;
        Log::Error(err.c_str());
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int ShaderProgram::CompileCompute(const std::string& sourcePath)
{
    std::string sourceCode = ReadFile(sourcePath);
    if (sourceCode.empty())
    {
        return 0;
    }

    // Compile shader
    unsigned int shaderId = rlCompileShader(sourceCode.c_str(), RL_COMPUTE_SHADER);
    if (shaderId == 0)
    {
        std::string err = "Compute shader compile failed: " + sourcePath + " (See Raylib trace for full compiler log)";
        Log::Error(err.c_str());
        return 0;
    }
    
    std::string succCompile = "Compute shader compiled successfully: " + sourcePath;
    Log::Info(succCompile.c_str());

    // Link program
    unsigned int programId = rlLoadComputeShaderProgram(shaderId);
    if (programId == 0)
    {
        std::string err = "Compute shader link failed: " + sourcePath + " (See Raylib trace for full linker log)";
        Log::Error(err.c_str());
        // rlLoadComputeShaderProgram doesn't delete the shader if it fails linking, but normally we would clean up.
        // For our purposes, returning 0 signals failure.
        return 0;
    }

    std::string succLink = "Compute shader linked successfully: " + sourcePath;
    Log::Info(succLink.c_str());

    return programId;
}

unsigned int ShaderProgram::CompileGraphics(const std::string& vsPath, const std::string& fsPath)
{
    std::string vsCode = ReadFile(vsPath);
    std::string fsCode = ReadFile(fsPath);
    
    if (vsCode.empty() || fsCode.empty())
    {
        return 0;
    }

    unsigned int vsId = rlCompileShader(vsCode.c_str(), RL_VERTEX_SHADER);
    if (vsId == 0)
    {
        Log::Error(("Vertex shader compile failed: " + vsPath).c_str());
        return 0;
    }

    unsigned int fsId = rlCompileShader(fsCode.c_str(), RL_FRAGMENT_SHADER);
    if (fsId == 0)
    {
        Log::Error(("Fragment shader compile failed: " + fsPath).c_str());
        return 0;
    }

    unsigned int programId = rlLoadShaderProgram(vsId, fsId);
    if (programId == 0)
    {
        Log::Error(("Graphics shader link failed: " + vsPath + " / " + fsPath).c_str());
        return 0;
    }

    Log::Info(("Graphics shader compiled and linked successfully: " + vsPath + " / " + fsPath).c_str());

    return programId;
}
