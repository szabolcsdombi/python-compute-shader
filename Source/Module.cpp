#include "OpenGL.hpp"

#include <Python.h>

char compiler_log[16 * 1024 + 1];

PyObject * ModuleError;

GLuint timerQuery;
bool timerStarted;

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

PyObject * StartTimer(PyObject * self, PyObject * args) {
	if (!PyArg_ParseTuple(args, ":StartTimer")) {
		return 0;
	}

	if (timerStarted) {
		glEndQuery(GL_TIME_ELAPSED);
	}
	
	glBeginQuery(GL_TIME_ELAPSED, timerQuery);
	timerStarted = true;

	Py_RETURN_NONE;
}

PyObject * GetTimer(PyObject * self, PyObject * args) {
	if (!PyArg_ParseTuple(args, ":GetTimer")) {
		return 0;
	}

	if (!timerStarted) {
		PyErr_SetString(ModuleError, "Timer not started");
		return 0;
	}

	glEndQuery(GL_TIME_ELAPSED);
	timerStarted = false;

	GLint available;
	while (!available) {
		glGetQueryObjectiv(timerQuery, GL_QUERY_RESULT_AVAILABLE, &available);
	}

	long long elapsed = 0;
	glGetQueryObjecti64v(timerQuery, GL_QUERY_RESULT, &elapsed);

	return PyFloat_FromDouble((double)elapsed / 1000000000);
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
	unsigned cs;
	unsigned binding = 0;

	if (!PyArg_ParseTuple(args, "II|I:UseSSBO", &ssbo, &cs, &binding)) {
		return 0;
	}

	glUseProgram(cs);
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
	{"StartTimer", StartTimer, METH_VARARGS, 0},
	{"GetTimer", GetTimer, METH_VARARGS, 0},
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


#define STR(a) #a
#define XSTR(a) STR(a)
#define MODULE(a) PyInit_ ## a
#define XMODULE(a) MODULE(a)

#ifndef MODULE_NAME
#define MODULE_NAME ComputeShader
#endif

static struct PyModuleDef moduledef = {PyModuleDef_HEAD_INIT, XSTR(MODULE_NAME), 0, -1, methods, 0, 0, 0, 0};

extern "C" {
	PyObject * XMODULE(MODULE_NAME)();
}

PyObject * XMODULE(MODULE_NAME)() {
	PyObject * module = PyModule_Create(&moduledef);
	if (!module) {
		return 0;
	}
	
	ModuleError = PyErr_NewException(XSTR(MODULE_NAME) ".Error", 0, 0);

	Py_INCREF(ModuleError);
	PyModule_AddObject(module, "Error", ModuleError);

	if (!InitializeGL()) {
		PyErr_SetString(ModuleError, GetError());
		return 0;
	}

	glGenQueries(1, &timerQuery);
	timerStarted = false;

	return module;
}
