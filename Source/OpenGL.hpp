#pragma once

#ifdef _WIN64
	typedef long long int GLsizeiptr;
	typedef long long int GLintptr;
#else
	typedef int GLsizeiptr;
	typedef int GLintptr;
#endif

typedef char GLbyte;
typedef char GLchar;
typedef double GLclampd;
typedef double GLdouble;
typedef float GLclampf;
typedef float GLfloat;
typedef int GLint;
typedef int GLsizei;
typedef long long int GLint64;
typedef short GLshort;
typedef struct __GLsync * GLsync;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;
typedef unsigned int GLbitfield;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef unsigned long long int GLuint64;
typedef unsigned short GLhalf;
typedef unsigned short GLushort;
typedef void GLvoid;

GLvoid __stdcall _glBindBuffer(GLenum target, GLuint buffer);
GLvoid __stdcall _glGenBuffers(GLsizei n, GLuint * buffers);
GLvoid __stdcall _glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
GLvoid __stdcall _glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data);
GLboolean __stdcall _glUnmapBuffer(GLenum target);
GLvoid __stdcall _glAttachShader(GLuint program, GLuint shader);
GLvoid __stdcall _glCompileShader(GLuint shader);
GLuint __stdcall _glCreateProgram();
GLuint __stdcall _glCreateShader(GLenum type);
GLvoid __stdcall _glDeleteProgram(GLuint program);
GLvoid __stdcall _glDeleteShader(GLuint shader);
GLvoid __stdcall _glDeleteBuffers(GLsizei n, const GLuint * buffers);
GLvoid __stdcall _glGetProgramiv(GLuint program, GLenum pname, GLint * params);
GLvoid __stdcall _glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
GLvoid __stdcall _glGetShaderiv(GLuint shader, GLenum pname, GLint * params);
GLvoid __stdcall _glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
GLvoid __stdcall _glLinkProgram(GLuint program);
GLvoid __stdcall _glShaderSource(GLuint shader, GLsizei count, const GLchar * const * string, const GLint * length);
GLvoid __stdcall _glUseProgram(GLuint program);
GLvoid __stdcall _glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
GLvoid * __stdcall _glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLvoid __stdcall _glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
GLvoid __stdcall _glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders);

extern decltype(_glBindBuffer) * glBindBuffer;
extern decltype(_glGenBuffers) * glGenBuffers;
extern decltype(_glBufferData) * glBufferData;
extern decltype(_glBufferSubData) * glBufferSubData;
extern decltype(_glUnmapBuffer) * glUnmapBuffer;
extern decltype(_glAttachShader) * glAttachShader;
extern decltype(_glCompileShader) * glCompileShader;
extern decltype(_glCreateProgram) * glCreateProgram;
extern decltype(_glCreateShader) * glCreateShader;
extern decltype(_glDeleteProgram) * glDeleteProgram;
extern decltype(_glDeleteShader) * glDeleteShader;
extern decltype(_glDeleteBuffers) * glDeleteBuffers;
extern decltype(_glGetProgramiv) * glGetProgramiv;
extern decltype(_glGetProgramInfoLog) * glGetProgramInfoLog;
extern decltype(_glGetShaderiv) * glGetShaderiv;
extern decltype(_glGetShaderInfoLog) * glGetShaderInfoLog;
extern decltype(_glLinkProgram) * glLinkProgram;
extern decltype(_glShaderSource) * glShaderSource;
extern decltype(_glUseProgram) * glUseProgram;
extern decltype(_glBindBufferBase) * glBindBufferBase;
extern decltype(_glMapBufferRange) * glMapBufferRange;
extern decltype(_glDispatchCompute) * glDispatchCompute;
extern decltype(_glGetAttachedShaders) * glGetAttachedShaders;

bool InitializeGL();
const char * GetError();
