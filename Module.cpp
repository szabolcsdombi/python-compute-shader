#include "OpenGL.hpp"

#include <Python.h>

const GLenum GL_COMPUTE_SHADER         = 0x91B9;
const GLenum GL_FALSE                  = 0x0000;
const GLenum GL_COMPILE_STATUS         = 0x8B81;
const GLenum GL_LINK_STATUS            = 0x8B82;
const GLenum GL_SHADER_STORAGE_BUFFER  = 0x90D2;
const GLenum GL_SHADER_STORAGE_BLOCK   = 0x92E6;
const GLenum GL_DYNAMIC_COPY           = 0x88EA;
const GLenum GL_MAP_READ_BIT           = 0x0001;

char compiler_log[16 * 1024 + 1];

PyObject * ModuleError;

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
	const char * name;
	unsigned cs = 0;

	if (!PyArg_ParseTuple(args, "IIs:UseSSBO", &ssbo, &cs, &name)) {
		return 0;
	}

	GLint index = glGetProgramResourceIndex(cs, GL_SHADER_STORAGE_BLOCK, name);

	if (index < 0) {
		PyErr_Format(ModuleError, "%s not found", name);
		return 0;
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ssbo);
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

static struct PyModuleDef moduledef = {PyModuleDef_HEAD_INIT, "ComputeShader", 0, -1, methods, 0, 0, 0, 0};

extern "C" {
	PyObject * PyInit_ComputeShader();
}

PyObject * PyInit_ComputeShader() {
	PyObject * module = PyModule_Create(&moduledef);
	if (!module) {
		return 0;
	}
	
	ModuleError = PyErr_NewException("ComputeShader.Error", 0, 0);

	Py_INCREF(ModuleError);
	PyModule_AddObject(module, "Error", ModuleError);

	if (!InitializeGL()) {
		PyErr_SetString(ModuleError, GetError());
		return 0;
	}

	return module;
}