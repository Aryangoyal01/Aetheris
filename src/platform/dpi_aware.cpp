#include <windows.h>

// DPI_AWARENESS values
#ifndef PROCESS_PER_MONITOR_DPI_AWARE
#define PROCESS_PER_MONITOR_DPI_AWARE 2
#endif

typedef long(__stdcall* SetProcessDpiAwarenessFunc)(int);

void SetAetherisDpiAware()
{
    HMODULE hShcore = LoadLibraryA("Shcore.dll");
    if (hShcore) {
        auto pFunc = (SetProcessDpiAwarenessFunc)GetProcAddress(hShcore, "SetProcessDpiAwareness");
        if (pFunc) {
            pFunc(PROCESS_PER_MONITOR_DPI_AWARE);
        }
        FreeLibrary(hShcore);
        return;
    }

    HMODULE hUser32 = LoadLibraryA("user32.dll");
    if (hUser32) {
        typedef int(__stdcall* SetProcessDPIAwareFunc)();
        auto pFunc = (SetProcessDPIAwareFunc)GetProcAddress(hUser32, "SetProcessDPIAware");
        if (pFunc) {
            pFunc();
        }
        FreeLibrary(hUser32);
    }
}
