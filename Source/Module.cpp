#include "OpenGL.hpp"

#include <Python.h>

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

char compiler_log[16 * 1024 + 1];

PyObject * ModuleError;

PyObject * Info(PyObject * self, PyObject * args) {
	if (!PyArg_ParseTuple(args, ":Info")) {
		return 0;
	}

	int ATOMIC_COUNTERS = 0;
	int SHARED_MEMORY_SIZE = 0;
	int SHADER_STORAGE_BLOCKS = 0;
	int SHADER_STORAGE_BUFFER_BINDINGS = 0;
	int WORK_GROUP_INVOCATIONS = 0;
	int WORK_GROUP_COUNT[3] = {0, 0, 0};
	int WORK_GROUP_SIZE[3] = {0, 0, 0};

	int UNIFORM_BLOCKS = 0;
	int UNIFORM_BLOCK_SIZE = 0;
	int UNIFORM_COMPONENTS = 0;
	int MAJOR_VERSION = 0;
	int MINOR_VERSION = 0;

	glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTERS, &ATOMIC_COUNTERS);
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &SHARED_MEMORY_SIZE);
	glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &SHADER_STORAGE_BLOCKS);
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &SHADER_STORAGE_BUFFER_BINDINGS);
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &WORK_GROUP_INVOCATIONS);

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &WORK_GROUP_COUNT[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &WORK_GROUP_COUNT[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &WORK_GROUP_COUNT[2]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &WORK_GROUP_SIZE[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &WORK_GROUP_SIZE[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &WORK_GROUP_SIZE[2]);

	glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &UNIFORM_BLOCKS);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &UNIFORM_BLOCK_SIZE);
	glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &UNIFORM_COMPONENTS);
	glGetIntegerv(GL_MAJOR_VERSION, &MAJOR_VERSION);
	glGetIntegerv(GL_MINOR_VERSION, &MINOR_VERSION);

	PyObject * result = PyDict_New();

	PyDict_SetItemString(result, "ATOMIC_COUNTERS", PyLong_FromLong(ATOMIC_COUNTERS));
	PyDict_SetItemString(result, "SHARED_MEMORY_SIZE", PyLong_FromLong(SHARED_MEMORY_SIZE));
	PyDict_SetItemString(result, "SHADER_STORAGE_BLOCKS", PyLong_FromLong(SHADER_STORAGE_BLOCKS));
	PyDict_SetItemString(result, "SHADER_STORAGE_BUFFER_BINDINGS", PyLong_FromLong(SHADER_STORAGE_BUFFER_BINDINGS));
	PyDict_SetItemString(result, "WORK_GROUP_INVOCATIONS", PyLong_FromLong(WORK_GROUP_INVOCATIONS));
	PyDict_SetItemString(result, "WORK_GROUP_COUNT", Py_BuildValue("{s:i, s:i, s:i}", "x", WORK_GROUP_COUNT[0], "y", WORK_GROUP_COUNT[1], "z", WORK_GROUP_COUNT[2]));
	PyDict_SetItemString(result, "WORK_GROUP_SIZE", Py_BuildValue("{s:i, s:i, s:i}", "x", WORK_GROUP_SIZE[0], "y", WORK_GROUP_SIZE[1], "z", WORK_GROUP_SIZE[2]));

	PyDict_SetItemString(result, "UNIFORM_BLOCKS", PyLong_FromLong(UNIFORM_BLOCKS));
	PyDict_SetItemString(result, "UNIFORM_BLOCK_SIZE", PyLong_FromLong(UNIFORM_BLOCK_SIZE));
	PyDict_SetItemString(result, "UNIFORM_COMPONENTS", PyLong_FromLong(UNIFORM_COMPONENTS));
	PyDict_SetItemString(result, "VERSION", Py_BuildValue("{s:i, s:i}", "MAJOR", MAJOR_VERSION, "MINOR", MINOR_VERSION));

	return result;
}

PyObject * NewCS(PyObject * self, PyObject * args) {
	const char * source;

	if (!PyArg_ParseTuple(args, "s:NewCS", &source)) {
		return 0;
	}

	GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	GLint compiled = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		int log_size = 0;
		glGetShaderInfoLog(shader, 16 * 1024, &log_size, compiler_log);
		compiler_log[log_size] = 0;
		glDeleteShader(shader);

		PyErr_SetString(ModuleError, compiler_log);
		return 0;
	} else {
		compiler_log[0] = 0;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, shader);
	glLinkProgram(program);

	GLint linked = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		int log_size = 0;
		glGetProgramInfoLog(program, 16 * 1024, &log_size, compiler_log);
		compiler_log[log_size] = 0;
		glDeleteProgram(program);
		glDeleteShader(shader);

		PyErr_SetString(ModuleError, compiler_log);
		return 0;
	} else {
		compiler_log[0] = 0;
	} 

	return PyLong_FromLong(program);
}

PyObject * UseCS(PyObject * self, PyObject * args) {
	unsigned cs;
	unsigned x = 1;
	unsigned y = 1;
	unsigned z = 1;

	if (!PyArg_ParseTuple(args, "I|III:UseCS", &cs, &x, &y, &z)) {
		return 0;
	}

	glUseProgram(cs);
	glDispatchCompute(x, y, z);
	Py_RETURN_NONE;
}

PyObject * DeleteCS(PyObject * self, PyObject * args) {
	unsigned cs;

	if (!PyArg_ParseTuple(args, "I:DeleteCS", &cs)) {
		return 0;
	}

	GLuint shader = 0;
	glGetAttachedShaders(cs, 1, 0, &shader);
	glDeleteProgram(cs);
	glDeleteShader(shader);
	Py_RETURN_NONE;
}

PyObject * NewSSBO(PyObject * self, PyObject * args) {
	const char * data;
	int size;

	if (!PyArg_ParseTuple(args, "y#:NewSSBO", &data, &size)) {
		return 0;
	}

	GLuint ssbo = 0;
	glGenBuffers(1, &ssbo);

	if (!ssbo) {
		PyErr_SetString(ModuleError, "Cannot create buffer");
		return 0;
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
	return PyLong_FromLong(ssbo);
}

PyObject * UseSSBO(PyObject * self, PyObject * args) {
	unsigned ssbo;
	unsigned binding = 0;

	if (!PyArg_ParseTuple(args, "I|I:UseSSBO", &ssbo, &binding)) {
		return 0;
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ssbo);
	Py_RETURN_NONE;
}

PyObject * UpdateSSBO(PyObject * self, PyObject * args) {
	unsigned ssbo;
	const char * data;
	int size;
	unsigned offset = 0;

	if (!PyArg_ParseTuple(args, "Iy#|I:UpdateSSBO", &ssbo, &data, &size, &offset)) {
		return 0;
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, (GLintptr)offset, size, data);
	Py_RETURN_NONE;
}

PyObject * ReadSSBO(PyObject * self, PyObject * args) {
	unsigned ssbo;
	unsigned size;
	unsigned offset = 0;

	if (!PyArg_ParseTuple(args, "II|I:ReadSSBO", &ssbo, &size, &offset)) {
		return 0;
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	const void * map = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, offset, size, GL_MAP_READ_BIT);

	if (!map) {
		PyErr_SetString(ModuleError, "Mapping failed");
		return 0;
	}

	PyObject * result = PyBytes_FromStringAndSize((const char *)map, size);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	return result;
}

PyObject * DeleteSSBO(PyObject * self, PyObject * args) {
	unsigned ssbo;

	if (!PyArg_ParseTuple(args, "I:DeleteSSBO", &ssbo)) {
		return 0;
	}

	glDeleteBuffers(1, &ssbo);
	Py_RETURN_NONE;
}


static PyMethodDef methods[] = {
	{"Info", Info, METH_VARARGS, 0},
	{"NewCS", NewCS, METH_VARARGS, 0},
	{"UseCS", UseCS, METH_VARARGS, 0},
	{"DeleteCS", DeleteCS, METH_VARARGS, 0},
	{"NewSSBO", NewSSBO, METH_VARARGS, 0},
	{"UseSSBO", UseSSBO, METH_VARARGS, 0},
	{"UpdateSSBO", UpdateSSBO, METH_VARARGS, 0},
	{"ReadSSBO", ReadSSBO, METH_VARARGS, 0},
	{"DeleteSSBO", DeleteSSBO, METH_VARARGS, 0},
	{0, 0},
};

#ifdef _WIN64

static struct PyModuleDef moduledef = {PyModuleDef_HEAD_INIT, "ComputeShader64", 0, -1, methods, 0, 0, 0, 0};

extern "C" {
	PyObject * PyInit_ComputeShader64();
}

#else

static struct PyModuleDef moduledef = {PyModuleDef_HEAD_INIT, "ComputeShader32", 0, -1, methods, 0, 0, 0, 0};

extern "C" {
	PyObject * PyInit_ComputeShader32();
}

#endif

#ifdef _WIN64

PyObject * PyInit_ComputeShader64() {
	PyObject * module = PyModule_Create(&moduledef);
	if (!module) {
		return 0;
	}
	
	ModuleError = PyErr_NewException("ComputeShader64.Error", 0, 0);

	Py_INCREF(ModuleError);
	PyModule_AddObject(module, "Error", ModuleError);

	if (!InitializeGL()) {
		PyErr_SetString(ModuleError, GetError());
		return 0;
	}

	return module;
}

#else

PyObject * PyInit_ComputeShader32() {
	PyObject * module = PyModule_Create(&moduledef);
	if (!module) {
		return 0;
	}
	
	ModuleError = PyErr_NewException("ComputeShader32.Error", 0, 0);

	Py_INCREF(ModuleError);
	PyModule_AddObject(module, "Error", ModuleError);

	if (!InitializeGL()) {
		PyErr_SetString(ModuleError, GetError());
		return 0;
	}

	return module;
}

#endif