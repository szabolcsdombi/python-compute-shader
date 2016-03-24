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


const GLenum GL_COMPUTE_SHADER                      = 0x91B9;
const GLenum GL_FALSE                               = 0x0000;
const GLenum GL_COMPILE_STATUS                      = 0x8B81;
const GLenum GL_LINK_STATUS                         = 0x8B82;
const GLenum GL_SHADER_STORAGE_BUFFER               = 0x90D2;
const GLenum GL_SHADER_STORAGE_BLOCK                = 0x92E6;
const GLenum GL_DYNAMIC_COPY                        = 0x88EA;
const GLenum GL_MAP_READ_BIT                        = 0x0001;

const GLenum GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS   = 0x90DB;
const GLenum GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS  = 0x90DD;
const GLenum GL_MAX_COMPUTE_SHARED_MEMORY_SIZE      = 0x8262;
const GLenum GL_MAX_COMPUTE_ATOMIC_COUNTERS         = 0x8265;
const GLenum GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS  = 0x90EB;
const GLenum GL_MAX_COMPUTE_WORK_GROUP_COUNT        = 0x91BE;
const GLenum GL_MAX_COMPUTE_WORK_GROUP_SIZE         = 0x91BF;

const GLenum GL_MAX_UNIFORM_BLOCK_SIZE              = 0x8A30;
const GLenum GL_MAX_COMPUTE_UNIFORM_BLOCKS          = 0x91BB;
const GLenum GL_MAX_COMPUTE_UNIFORM_COMPONENTS      = 0x8263;
const GLenum GL_MAJOR_VERSION                       = 0x821B;
const GLenum GL_MINOR_VERSION                       = 0x821C;

const GLenum GL_QUERY_RESULT                        = 0x8866;
const GLenum GL_QUERY_RESULT_AVAILABLE              = 0x8867;
const GLenum GL_TIME_ELAPSED                        = 0x88BF;

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
GLvoid __stdcall _glGetIntegeri_v(GLenum target, GLuint index, GLint * data);
GLvoid __stdcall _glGetQueryObjecti64v(GLuint id, GLenum pname, GLint64 * params);
GLvoid __stdcall _glGetQueryObjectiv(GLuint id, GLenum pname, GLint * params);

GLvoid __stdcall _glGetIntegerv(GLenum pname, GLint * data);
GLenum __stdcall _glGetError();

GLvoid __stdcall _glGenQueries(GLsizei n, GLuint * ids);
GLvoid __stdcall _glBeginQuery(GLenum target, GLuint id);
GLvoid __stdcall _glEndQuery(GLenum target);


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
extern decltype(_glGetIntegeri_v) * glGetIntegeri_v;
extern decltype(_glGetQueryObjecti64v) * glGetQueryObjecti64v;
extern decltype(_glGetQueryObjectiv) * glGetQueryObjectiv;

extern decltype(_glGetIntegerv) * glGetIntegerv;
extern decltype(_glGetError) * glGetError;

extern decltype(_glGenQueries) * glGenQueries;
extern decltype(_glBeginQuery) * glBeginQuery;
extern decltype(_glEndQuery) * glEndQuery;


bool InitializeGL();
const char * GetError();
