#include "OpenGL.hpp"

#include <Windows.h>

decltype(_glBindBuffer) * glBindBuffer;
decltype(_glGenBuffers) * glGenBuffers;
decltype(_glBufferData) * glBufferData;
decltype(_glBufferSubData) * glBufferSubData;
decltype(_glUnmapBuffer) * glUnmapBuffer;
decltype(_glAttachShader) * glAttachShader;
decltype(_glCompileShader) * glCompileShader;
decltype(_glCreateProgram) * glCreateProgram;
decltype(_glCreateShader) * glCreateShader;
decltype(_glDeleteProgram) * glDeleteProgram;
decltype(_glDeleteShader) * glDeleteShader;
decltype(_glDeleteBuffers) * glDeleteBuffers;
decltype(_glGetProgramiv) * glGetProgramiv;
decltype(_glGetProgramInfoLog) * glGetProgramInfoLog;
decltype(_glGetShaderiv) * glGetShaderiv;
decltype(_glGetShaderInfoLog) * glGetShaderInfoLog;
decltype(_glLinkProgram) * glLinkProgram;
decltype(_glShaderSource) * glShaderSource;
decltype(_glUseProgram) * glUseProgram;
decltype(_glBindBufferBase) * glBindBufferBase;
decltype(_glMapBufferRange) * glMapBufferRange;
decltype(_glDispatchCompute) * glDispatchCompute;
decltype(_glGetAttachedShaders) * glGetAttachedShaders;
decltype(_glGetIntegeri_v) * glGetIntegeri_v;
decltype(_glGetQueryObjecti64v) * glGetQueryObjecti64v;
decltype(_glGetQueryObjectiv) * glGetQueryObjectiv;

decltype(_glGetIntegerv) * glGetIntegerv;
decltype(_glGetError) * glGetError;

decltype(_glGenQueries) * glGenQueries;
decltype(_glBeginQuery) * glBeginQuery;
decltype(_glEndQuery) * glEndQuery;

PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),	// nSize
	1,								// nVersion
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_GENERIC_ACCELERATED |
	PFD_DOUBLEBUFFER,				// dwFlags
	0,								// iPixelType
	32,								// cColorBits
	0,								// cRedBits
	0,								// cRedShift
	0,								// cGreenBits
	0,								// cGreenShift
	0,								// cBlueBits
	0,								// cBlueShift
	0,								// cAlphaBits
	0,								// cAlphaShift
	32,								// cAccumBits
	0,								// cAccumRedBits
	0,								// cAccumGreenBits
	0,								// cAccumBlueBits
	0,								// cAccumAlphaBits
	32,								// cDepthBits
	32,								// cStencilBits
	0,								// cAuxBuffers
	0,								// iLayerType
	0,								// bReserved
	0,								// dwLayerMask
	0,								// dwVisibleMask
	0								// dwDamageMask
};

const char * errorMessage = "";

bool InitializeGL() {
	HMODULE hinst = GetModuleHandle(0);

	if (!hinst) {
		errorMessage = "GetModuleHandle failed.";
		return false;
	}

	WNDCLASS extClass = {
		CS_OWNDC,						// style
		DefWindowProc,					// lpfnWndProc
		0,								// cbClsExtra
		0,								// cbWndExtra
		hinst,							// hInstance
		0,								// hIcon
		0,								// hCursor
		0,								// hbrBackground
		0,								// lpszMenuName
		"OpenGL",						// lpszClassName
	};

	if (!RegisterClass(&extClass)) {
		errorMessage = "RegisterClass failed.";
		return false;
	}

	HWND loader_hwnd = CreateWindowEx(
		0,
		"OpenGL",						// lpClassName
		0,								// lpWindowName
		0,								// dwStyle
		0,								// x
		0,								// y
		0,								// nWidth
		0,								// nHeight
		0,								// hWndParent
		0,								// hMenu
		hinst,							// hInstance
		0								// lpParam
	);

	if (!loader_hwnd) {
		errorMessage = "CreateWindowEx failed.";
		return false;
	}

	HDC loader_hdc = GetDC(loader_hwnd);

	if (!loader_hdc) {
		errorMessage = "GetDC failed.";
		return false;
	}

	int loader_pixelformat = ChoosePixelFormat(loader_hdc, &pfd);

	if (!loader_pixelformat) {
		errorMessage = "ChoosePixelFormat failed.";
		return false;
	}

	if (!SetPixelFormat(loader_hdc, loader_pixelformat, &pfd)) {
		errorMessage = "SetPixelFormat failed.";
		return false;
	}

	HGLRC loader_hglrc = wglCreateContext(loader_hdc);

	if (!loader_hglrc) {
		errorMessage = "wglCreateContext failed.";
		return false;
	}

	if (!wglMakeCurrent(loader_hdc, loader_hglrc)) {
		errorMessage = "wglMakeCurrent failed.";
		return false;
	}

	HMODULE opengl32 = LoadLibrary("opengl32.dll");
	glGetIntegerv = (decltype(_glGetIntegerv) *)GetProcAddress(opengl32, "glGetIntegerv");
	glGetError = (decltype(_glGetError) *)GetProcAddress(opengl32, "glGetError");

	glGenQueries = (decltype(_glGenQueries) *)wglGetProcAddress("glGenQueries");
	glBeginQuery = (decltype(_glBeginQuery) *)wglGetProcAddress("glBeginQuery");
	glEndQuery = (decltype(_glEndQuery) *)wglGetProcAddress("glEndQuery");

	glBindBuffer = (decltype(_glBindBuffer) *)wglGetProcAddress("glBindBuffer");
	glGenBuffers = (decltype(_glGenBuffers) *)wglGetProcAddress("glGenBuffers");
	glBufferData = (decltype(_glBufferData) *)wglGetProcAddress("glBufferData");
	glBufferSubData = (decltype(_glBufferSubData) *)wglGetProcAddress("glBufferSubData");
	glUnmapBuffer = (decltype(_glUnmapBuffer) *)wglGetProcAddress("glUnmapBuffer");
	glAttachShader = (decltype(_glAttachShader) *)wglGetProcAddress("glAttachShader");
	glCompileShader = (decltype(_glCompileShader) *)wglGetProcAddress("glCompileShader");
	glCreateProgram = (decltype(_glCreateProgram) *)wglGetProcAddress("glCreateProgram");
	glCreateShader = (decltype(_glCreateShader) *)wglGetProcAddress("glCreateShader");
	glDeleteProgram = (decltype(_glDeleteProgram) *)wglGetProcAddress("glDeleteProgram");
	glDeleteShader = (decltype(_glDeleteShader) *)wglGetProcAddress("glDeleteShader");
	glDeleteBuffers = (decltype(_glDeleteBuffers) *)wglGetProcAddress("glDeleteBuffers");
	glGetProgramiv = (decltype(_glGetProgramiv) *)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (decltype(_glGetProgramInfoLog) *)wglGetProcAddress("glGetProgramInfoLog");
	glGetShaderiv = (decltype(_glGetShaderiv) *)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (decltype(_glGetShaderInfoLog) *)wglGetProcAddress("glGetShaderInfoLog");
	glLinkProgram = (decltype(_glLinkProgram) *)wglGetProcAddress("glLinkProgram");
	glShaderSource = (decltype(_glShaderSource) *)wglGetProcAddress("glShaderSource");
	glUseProgram = (decltype(_glUseProgram) *)wglGetProcAddress("glUseProgram");
	glBindBufferBase = (decltype(_glBindBufferBase) *)wglGetProcAddress("glBindBufferBase");
	glMapBufferRange = (decltype(_glMapBufferRange) *)wglGetProcAddress("glMapBufferRange");
	glDispatchCompute = (decltype(_glDispatchCompute) *)wglGetProcAddress("glDispatchCompute");
	glGetAttachedShaders = (decltype(_glGetAttachedShaders) *)wglGetProcAddress("glGetAttachedShaders");
	glGetIntegeri_v = (decltype(_glGetIntegeri_v) *)wglGetProcAddress("glGetIntegeri_v");
	glGetQueryObjecti64v = (decltype(_glGetQueryObjecti64v) *)wglGetProcAddress("glGetQueryObjecti64v");
	glGetQueryObjectiv = (decltype(_glGetQueryObjectiv) *)wglGetProcAddress("glGetQueryObjectiv");

	if (!glBindBuffer) {
		errorMessage = "glBindBuffer not loaded!";
		return false;
	}

	if (!glGenBuffers) {
		errorMessage = "glGenBuffers not loaded!";
		return false;
	}

	if (!glBufferData) {
		errorMessage = "glBufferData not loaded!";
		return false;
	}

	if (!glBufferSubData) {
		errorMessage = "glBufferSubData not loaded!";
		return false;
	}

	if (!glUnmapBuffer) {
		errorMessage = "glUnmapBuffer not loaded!";
		return false;
	}

	if (!glAttachShader) {
		errorMessage = "glAttachShader not loaded!";
		return false;
	}

	if (!glCompileShader) {
		errorMessage = "glCompileShader not loaded!";
		return false;
	}

	if (!glCreateProgram) {
		errorMessage = "glCreateProgram not loaded!";
		return false;
	}

	if (!glCreateShader) {
		errorMessage = "glCreateShader not loaded!";
		return false;
	}

	if (!glDeleteProgram) {
		errorMessage = "glDeleteProgram not loaded!";
		return false;
	}

	if (!glDeleteShader) {
		errorMessage = "glDeleteShader not loaded!";
		return false;
	}

	if (!glDeleteBuffers) {
		errorMessage = "glDeleteBuffers not loaded!";
		return false;
	}

	if (!glGetProgramiv) {
		errorMessage = "glGetProgramiv not loaded!";
		return false;
	}

	if (!glGetProgramInfoLog) {
		errorMessage = "glGetProgramInfoLog not loaded!";
		return false;
	}

	if (!glGetShaderiv) {
		errorMessage = "glGetShaderiv not loaded!";
		return false;
	}

	if (!glGetShaderInfoLog) {
		errorMessage = "glGetShaderInfoLog not loaded!";
		return false;
	}

	if (!glLinkProgram) {
		errorMessage = "glLinkProgram not loaded!";
		return false;
	}

	if (!glShaderSource) {
		errorMessage = "glShaderSource not loaded!";
		return false;
	}

	if (!glUseProgram) {
		errorMessage = "glUseProgram not loaded!";
		return false;
	}

	if (!glBindBufferBase) {
		errorMessage = "glBindBufferBase not loaded!";
		return false;
	}

	if (!glMapBufferRange) {
		errorMessage = "glMapBufferRange not loaded!";
		return false;
	}

	if (!glDispatchCompute) {
		errorMessage = "glDispatchCompute not loaded!";
		return false;
	}

	if (!glGetAttachedShaders) {
		errorMessage = "glGetAttachedShaders not loaded!";
		return false;
	}

	if (!glGetIntegeri_v) {
		errorMessage = "glGetIntegeri_v not loaded!";
		return false;
	}

	if (!glGetQueryObjecti64v) {
		errorMessage = "glGetQueryObjecti64v not loaded!";
		return false;
	}

	if (!glGetQueryObjectiv) {
		errorMessage = "glGetQueryObjectiv not loaded!";
		return false;
	}

	if (!glGetIntegerv) {
		errorMessage = "glGetIntegerv not loaded!";
		return false;
	}

	if (!glGetError) {
		errorMessage = "glGetError not loaded!";
		return false;
	}

	if (!glGenQueries) {
		errorMessage = "glGenQueries not loaded!";
		return false;
	}

	if (!glBeginQuery) {
		errorMessage = "glBeginQuery not loaded!";
		return false;
	}

	if (!glEndQuery) {
		errorMessage = "glEndQuery not loaded!";
		return false;
	}
	
	return true;
}

const char * GetError() {
	return errorMessage;
}
