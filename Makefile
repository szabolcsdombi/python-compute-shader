C32 = g++ -std=c++11
C64 = x64_g++ -std=c++11

L32 = g++ -O3
L64 = x64_g++ -O3

PY = python
MK = make

PYTHON_PATH_32 = C:\Users\User\AppData\Local\Programs\Python\Python35-32
PYTHON_PATH_64 = C:\Users\User\AppData\Local\Programs\Python\Python35

PY_INC_32 = -I $(PYTHON_PATH_32)\include
PY_LIB_32 = -L $(PYTHON_PATH_32)\libs

PY_INC_64 = -I $(PYTHON_PATH_64)\include
PY_LIB_64 = -L $(PYTHON_PATH_64)\libs

LIBS = -lgdi32 -lopengl32 -lpython35

PY = python

all: clean Bin\ComputeShader32.pyd Bin\ComputeShader64.pyd release-beta

prepare:
	$(PY) Prepare.py

clean:
	$(PY) Clear.py

release-beta:
	$(PY) Release.py

Bin\ComputeShader32.pyd: prepare Temp\Module32.o Temp\OpenGL32.o Source\Module32.def
	$(L32) -shared Source\Module32.def Temp\Module32.o Temp\OpenGL32.o $(PY_LIB_32) $(LIBS) -o Bin\ComputeShader32.pyd

Bin\ComputeShader64.pyd: prepare Temp\Module64.o Temp\OpenGL64.o Source\Module64.def
	$(L64) -shared Source\Module64.def Temp\Module64.o Temp\OpenGL64.o $(PY_LIB_64) $(LIBS) -o Bin\ComputeShader64.pyd

Temp\Module32.o: prepare Source\Module.cpp Source\OpenGL.hpp
	$(C32) -c $(PY_INC_32) Source\Module.cpp -o Temp\Module32.o
	
Temp\OpenGL32.o: prepare Source\OpenGL.cpp Source\OpenGL.hpp
	$(C32) -c $(PY_INC_32) Source\OpenGL.cpp -o Temp\OpenGL32.o

Temp\Module64.o: prepare Source\Module.cpp Source\OpenGL.hpp
	$(C64) -c $(PY_INC_64) Source\Module.cpp -o Temp\Module64.o
	
Temp\OpenGL64.o: prepare Source\OpenGL.cpp Source\OpenGL.hpp
	$(C64) -c $(PY_INC_64) Source\OpenGL.cpp -o Temp\OpenGL64.o

Run:
	$(PY) Test.py
