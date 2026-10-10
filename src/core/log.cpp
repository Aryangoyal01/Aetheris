#include "log.h"
#include <cstdio>
#include <ctime>
#include <mutex>
#include <vector>

static std::vector<LogEntry> s_entries;
static std::mutex s_logMutex;
constexpr size_t MAX_LOG_ENTRIES = 2000;

static std::string GetCurrentTimestamp()
{
    std::time_t now = std::time(nullptr);
    std::tm tmNow{};
#if defined(_WIN32)
    localtime_s(&tmNow, &now);
#else
    localtime_r(&now, &tmNow);
#endif
    char buf[16];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", &tmNow);
    return std::string(buf);
}

static void AddEntry(LogLevel level, const char* message)
{
    std::lock_guard<std::mutex> lock(s_logMutex);
    if (s_entries.size() >= MAX_LOG_ENTRIES)
    {
        s_entries.erase(s_entries.begin());
    }
    s_entries.push_back({ GetCurrentTimestamp(), level, message ? message : "" });
}

void Log::Info(const char* message)
{
    std::printf("[INFO] %s\n", message);
    std::fflush(stdout);
    AddEntry(LogLevel::Info, message);
}

void Log::Warning(const char* message)
{
    std::printf("[WARN] %s\n", message);
    std::fflush(stdout);
    AddEntry(LogLevel::Warning, message);
}

void Log::Error(const char* message)
{
    std::fprintf(stderr, "[ERROR] %s\n", message);
    std::fflush(stderr);
    AddEntry(LogLevel::Error, message);
}

void Log::System(const char* message)
{
    std::printf("[SYSTEM] %s\n", message);
    std::fflush(stdout);
    AddEntry(LogLevel::System, message);
}

const std::vector<LogEntry>& Log::GetEntries()
{
    std::lock_guard<std::mutex> lock(s_logMutex);
    return s_entries;
}

void Log::Clear()
{
    std::lock_guard<std::mutex> lock(s_logMutex);
    s_entries.clear();
}
