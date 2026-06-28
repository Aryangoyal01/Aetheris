#include "log.h"
#include <cstdio>

void Log::Info(const char* message)
{
    std::printf("[INFO] %s\n", message);
    std::fflush(stdout);
}

void Log::Warning(const char* message)
{
    std::printf("[WARN] %s\n", message);
    std::fflush(stdout);
}

void Log::Error(const char* message)
{
    std::fprintf(stderr, "[ERROR] %s\n", message);
    std::fflush(stderr);
}
