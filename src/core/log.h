#pragma once

#include <string>
#include <vector>

enum class LogLevel
{
    Info,
    Warning,
    Error,
    System
};

struct LogEntry
{
    std::string timestamp;
    LogLevel level;
    std::string message;
};

class Log
{
public:
    static void Info(const char* message);
    static void Warning(const char* message);
    static void Error(const char* message);
    static void System(const char* message);

    static const std::vector<LogEntry>& GetEntries();
    static void Clear();
};
