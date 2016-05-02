COMP = g++ -std=c++11 -c
LINK = g++ -O3

PY = python
MK = make

BITS = 32
PY_TARGET = Python35

PY_HOME = D:\PythonBuild\$(PY_TARGET)-$(BITS)

# by default the python3.5 installation folder is
# C:\Python35
# PY_HOME = C:\$(PY_TARGET)

# or

# C:\Python35-64
# PY_HOME = C:\$(PY_TARGET)-$(BITS)


PY_INC = -I $(PY_HOME)\include
PY_DLL = $(PY_HOME)\$(PY_TARGET).dll

DEPS = -lgdi32 -lopengl32 $(PY_DLL)

all: clean Bin\ComputeShader.pyd release-beta

prepare:
	$(PY) Prepare.py

clean:
	$(PY) Clear.py

release-beta:
	$(PY) Release.py beta $(PY_TARGET) $(BITS)

Bin\ComputeShader.pyd: prepare Temp\Module.o Temp\OpenGL.o Source\Module.def
	$(LINK) -shared Temp\Module.o Temp\OpenGL.o $(DEPS) -o Bin\ComputeShader.pyd

Temp\Module.o: prepare Source\Module.cpp Source\OpenGL.hpp
	$(COMP) $(PY_INC) Source\Module.cpp -o Temp\Module.o
	
Temp\OpenGL.o: prepare Source\OpenGL.cpp Source\OpenGL.hpp
	$(COMP) $(PY_INC) Source\OpenGL.cpp -o Temp\OpenGL.o

Run:
	$(PY) Test.py
