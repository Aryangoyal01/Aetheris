#pragma once

class Log
{
public:
    static void Info(const char* message);
    static void Warning(const char* message);
    static void Error(const char* message);
};
