#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*PFNGLGenVertexArraysPROC)(unsigned int n, unsigned int* arrays);
typedef void (*PFNGLBindVertexArrayPROC)(unsigned int array);
typedef void (*PFNGLDeleteVertexArraysPROC)(unsigned int n, const unsigned int* arrays);
typedef void (*PFNGLGenBuffersPROC)(unsigned int n, unsigned int* buffers);
typedef void (*PFNGLBindBufferPROC)(unsigned int target, unsigned int buffer);
typedef void (*PFNGLBufferDataPROC)(unsigned int target, int size, const void* data, unsigned int usage);
typedef void (*PFNGLDeleteBuffersPROC)(unsigned int n, const unsigned int* buffers);
typedef void (*PFNGLVertexAttribPointerPROC)(unsigned int index, int size, unsigned int type, unsigned char normalized, int stride, const void* pointer);
typedef void (*PFNGLEnableVertexAttribArrayPROC)(unsigned int index);
typedef void (*PFNGLDisableVertexAttribArrayPROC)(unsigned int index);
typedef void (*PFNGLDrawElementsInstancedPROC)(unsigned int mode, int count, unsigned int type, const void* indices, int instancecount);
typedef void (*PFNGLVertexAttribDivisorPROC)(unsigned int index, unsigned int divisor);
typedef void (*PFNGLMemoryBarrierPROC)(unsigned int barriers);
typedef void (*PFNGLBindBufferBasePROC)(unsigned int target, unsigned int index, unsigned int buffer);
typedef const unsigned char* (*PFNGLGetStringPROC)(unsigned int name);
typedef unsigned int (*PFNGLGetErrorPROC)(void);
typedef void (*PFNGLUseProgramPROC)(unsigned int program);
typedef unsigned int (*PFNGLCreateShaderPROC)(unsigned int type);
typedef void (*PFNGLShaderSourcePROC)(unsigned int shader, int count, const char* const* string, const int* length);
typedef void (*PFNGLCompileShaderPROC)(unsigned int shader);
typedef void (*PFNGLGetShaderivPROC)(unsigned int shader, unsigned int pname, int* params);
typedef void (*PFNGLGetShaderInfoLogPROC)(unsigned int shader, int bufSize, int* length, char* infoLog);
typedef void (*PFNGLDeleteShaderPROC)(unsigned int shader);
typedef unsigned int (*PFNGLCreateProgramPROC)(void);
typedef void (*PFNGLAttachShaderPROC)(unsigned int program, unsigned int shader);
typedef void (*PFNGLLinkProgramPROC)(unsigned int program);
typedef void (*PFNGLGetProgramivPROC)(unsigned int program, unsigned int pname, int* params);
typedef void (*PFNGLDeleteProgramPROC)(unsigned int program);
typedef int (*PFNGLGetUniformLocationPROC)(unsigned int program, const char* name);
typedef void (*PFNGLUniformMatrix4fvPROC)(unsigned int location, int count, unsigned char transpose, const float* value);
typedef void (*PFNGLUniformMatrix3fvPROC)(unsigned int location, int count, unsigned char transpose, const float* value);
typedef void (*PFNGLUniform3fPROC)(unsigned int location, float x, float y, float z);

extern PFNGLGenVertexArraysPROC ext_glGenVertexArrays;
extern PFNGLBindVertexArrayPROC ext_glBindVertexArray;
extern PFNGLDeleteVertexArraysPROC ext_glDeleteVertexArrays;
extern PFNGLGenBuffersPROC ext_glGenBuffers;
extern PFNGLBindBufferPROC ext_glBindBuffer;
extern PFNGLBufferDataPROC ext_glBufferData;
extern PFNGLDeleteBuffersPROC ext_glDeleteBuffers;
extern PFNGLVertexAttribPointerPROC ext_glVertexAttribPointer;
extern PFNGLEnableVertexAttribArrayPROC ext_glEnableVertexAttribArray;
extern PFNGLDisableVertexAttribArrayPROC ext_glDisableVertexAttribArray;
extern PFNGLDrawElementsInstancedPROC ext_glDrawElementsInstanced;
extern PFNGLVertexAttribDivisorPROC ext_glVertexAttribDivisor;
extern PFNGLMemoryBarrierPROC ext_glMemoryBarrier;
extern PFNGLBindBufferBasePROC ext_glBindBufferBase;
extern PFNGLGetStringPROC ext_glGetString;
extern PFNGLGetErrorPROC ext_glGetError;
extern PFNGLUseProgramPROC ext_glUseProgram;
extern PFNGLCreateShaderPROC ext_glCreateShader;
extern PFNGLShaderSourcePROC ext_glShaderSource;
extern PFNGLCompileShaderPROC ext_glCompileShader;
extern PFNGLGetShaderivPROC ext_glGetShaderiv;
extern PFNGLGetShaderInfoLogPROC ext_glGetShaderInfoLog;
extern PFNGLDeleteShaderPROC ext_glDeleteShader;
extern PFNGLCreateProgramPROC ext_glCreateProgram;
extern PFNGLAttachShaderPROC ext_glAttachShader;
extern PFNGLLinkProgramPROC ext_glLinkProgram;
extern PFNGLGetProgramivPROC ext_glGetProgramiv;
extern PFNGLDeleteProgramPROC ext_glDeleteProgram;
extern PFNGLGetUniformLocationPROC ext_glGetUniformLocation;
extern PFNGLUniformMatrix4fvPROC ext_glUniformMatrix4fv;
extern PFNGLUniformMatrix3fvPROC ext_glUniformMatrix3fv;
extern PFNGLUniform3fPROC ext_glUniform3f;

#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_TRIANGLES 0x0004
#define GL_UNSIGNED_INT 0x1405
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BARRIER_BIT 0x2000
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_VERSION 0x1F02
#define GL_RENDERER 0x1F01
#define GL_VENDOR 0x1F00
#define GL_NO_ERROR 0
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84

int LoadGLFunctions(void);

#ifdef __cplusplus
}
#endif
