#include "gl_helpers.h"
#include <windows.h>
#include <cstdio>

PFNGLGenVertexArraysPROC ext_glGenVertexArrays = nullptr;
PFNGLBindVertexArrayPROC ext_glBindVertexArray = nullptr;
PFNGLDeleteVertexArraysPROC ext_glDeleteVertexArrays = nullptr;
PFNGLGenBuffersPROC ext_glGenBuffers = nullptr;
PFNGLBindBufferPROC ext_glBindBuffer = nullptr;
PFNGLBufferDataPROC ext_glBufferData = nullptr;
PFNGLDeleteBuffersPROC ext_glDeleteBuffers = nullptr;
PFNGLVertexAttribPointerPROC ext_glVertexAttribPointer = nullptr;
PFNGLEnableVertexAttribArrayPROC ext_glEnableVertexAttribArray = nullptr;
PFNGLDisableVertexAttribArrayPROC ext_glDisableVertexAttribArray = nullptr;
PFNGLDrawElementsInstancedPROC ext_glDrawElementsInstanced = nullptr;
PFNGLVertexAttribDivisorPROC ext_glVertexAttribDivisor = nullptr;
PFNGLMemoryBarrierPROC ext_glMemoryBarrier = nullptr;
PFNGLBindBufferBasePROC ext_glBindBufferBase = nullptr;
PFNGLGetStringPROC ext_glGetString = nullptr;
PFNGLGetErrorPROC ext_glGetError = nullptr;
PFNGLUseProgramPROC ext_glUseProgram = nullptr;
PFNGLCreateShaderPROC ext_glCreateShader = nullptr;
PFNGLShaderSourcePROC ext_glShaderSource = nullptr;
PFNGLCompileShaderPROC ext_glCompileShader = nullptr;
PFNGLGetShaderivPROC ext_glGetShaderiv = nullptr;
PFNGLGetShaderInfoLogPROC ext_glGetShaderInfoLog = nullptr;
PFNGLDeleteShaderPROC ext_glDeleteShader = nullptr;
PFNGLCreateProgramPROC ext_glCreateProgram = nullptr;
PFNGLAttachShaderPROC ext_glAttachShader = nullptr;
PFNGLLinkProgramPROC ext_glLinkProgram = nullptr;
PFNGLGetProgramivPROC ext_glGetProgramiv = nullptr;
PFNGLDeleteProgramPROC ext_glDeleteProgram = nullptr;
PFNGLGetUniformLocationPROC ext_glGetUniformLocation = nullptr;
PFNGLUniformMatrix4fvPROC ext_glUniformMatrix4fv = nullptr;
PFNGLUniformMatrix3fvPROC ext_glUniformMatrix3fv = nullptr;
PFNGLUniform3fPROC ext_glUniform3f = nullptr;

int LoadGLFunctions(void) {
    HMODULE raylib = GetModuleHandleA("libraylib.dll");
    if (!raylib) raylib = GetModuleHandleA(nullptr);
    if (!raylib) return 0;

    HMODULE opengl32 = LoadLibraryA("opengl32.dll");

    int loaded = 0;
    int failed = 0;

    auto loadRaylib = [&](const char* glName, void** out) {
        char buf[128];
        snprintf(buf, sizeof(buf), "glad_%s", glName);
        void** ppGlobal = (void**)GetProcAddress(raylib, buf);
        if (ppGlobal && *ppGlobal) {
            *out = *ppGlobal;
            loaded++;
        } else {
            failed++;
        }
    };

    auto loadGL32 = [&](const char* glName, void** out) {
        *out = (void*)GetProcAddress(opengl32, glName);
        if (*out) loaded++; else failed++;
    };

    loadRaylib("glGenVertexArrays", (void**)&ext_glGenVertexArrays);
    loadRaylib("glBindVertexArray", (void**)&ext_glBindVertexArray);
    loadRaylib("glDeleteVertexArrays", (void**)&ext_glDeleteVertexArrays);
    loadRaylib("glGenBuffers", (void**)&ext_glGenBuffers);
    loadRaylib("glBindBuffer", (void**)&ext_glBindBuffer);
    loadRaylib("glBufferData", (void**)&ext_glBufferData);
    loadRaylib("glDeleteBuffers", (void**)&ext_glDeleteBuffers);
    loadRaylib("glVertexAttribPointer", (void**)&ext_glVertexAttribPointer);
    loadRaylib("glEnableVertexAttribArray", (void**)&ext_glEnableVertexAttribArray);
    loadRaylib("glDisableVertexAttribArray", (void**)&ext_glDisableVertexAttribArray);
    loadRaylib("glDrawElementsInstanced", (void**)&ext_glDrawElementsInstanced);
    loadRaylib("glVertexAttribDivisor", (void**)&ext_glVertexAttribDivisor);
    loadRaylib("glMemoryBarrier", (void**)&ext_glMemoryBarrier);
    loadRaylib("glBindBufferBase", (void**)&ext_glBindBufferBase);
    loadGL32("glGetString", (void**)&ext_glGetString);
    loadGL32("glGetError", (void**)&ext_glGetError);
    loadRaylib("glUseProgram", (void**)&ext_glUseProgram);
    loadRaylib("glCreateShader", (void**)&ext_glCreateShader);
    loadRaylib("glShaderSource", (void**)&ext_glShaderSource);
    loadRaylib("glCompileShader", (void**)&ext_glCompileShader);
    loadRaylib("glGetShaderiv", (void**)&ext_glGetShaderiv);
    loadRaylib("glGetShaderInfoLog", (void**)&ext_glGetShaderInfoLog);
    loadRaylib("glDeleteShader", (void**)&ext_glDeleteShader);
    loadRaylib("glCreateProgram", (void**)&ext_glCreateProgram);
    loadRaylib("glAttachShader", (void**)&ext_glAttachShader);
    loadRaylib("glLinkProgram", (void**)&ext_glLinkProgram);
    loadRaylib("glGetProgramiv", (void**)&ext_glGetProgramiv);
    loadRaylib("glDeleteProgram", (void**)&ext_glDeleteProgram);
    loadRaylib("glGetUniformLocation", (void**)&ext_glGetUniformLocation);
    loadRaylib("glUniformMatrix4fv", (void**)&ext_glUniformMatrix4fv);
    loadRaylib("glUniformMatrix3fv", (void**)&ext_glUniformMatrix3fv);
    loadRaylib("glUniform3f", (void**)&ext_glUniform3f);

    int required = ext_glGenVertexArrays && ext_glBindVertexArray && ext_glGenBuffers &&
                   ext_glBindBuffer && ext_glBufferData && ext_glDeleteBuffers &&
                   ext_glVertexAttribPointer && ext_glEnableVertexAttribArray &&
                   ext_glDrawElementsInstanced && ext_glDeleteVertexArrays &&
                   ext_glMemoryBarrier && ext_glBindBufferBase && ext_glGetString &&
                   ext_glGetError && ext_glUseProgram &&
                   ext_glCreateShader && ext_glShaderSource && ext_glCompileShader &&
                   ext_glGetShaderiv && ext_glDeleteShader &&
                   ext_glCreateProgram && ext_glAttachShader && ext_glLinkProgram &&
                   ext_glGetProgramiv && ext_glDeleteProgram &&
                   ext_glGetUniformLocation && ext_glUniformMatrix4fv &&
                   ext_glUniformMatrix3fv && ext_glUniform3f;

    return required ? 1 : 0;
}
