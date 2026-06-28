#pragma once

#include <string>

class ShaderProgram
{
public:
    static unsigned int CompileCompute(const std::string& sourcePath);
    static unsigned int CompileGraphics(const std::string& vsPath, const std::string& fsPath);

private:
    static std::string ReadFile(const std::string& path);
};
