PYTHON_PATH_32 = C:\Users\User\AppData\Local\Programs\Python\Python35-32
PY_INC_32 = -I $(PYTHON_PATH_32)\include
PY_LIB_32 = -L $(PYTHON_PATH_32)\libs

FLAGS = -std=c++11 -O3 $(PY_INC_32) $(PY_LIB_32)

PY = python

all:
	g++ -shared $(FLAGS) Module.def Module.cpp OpenGL.cpp -lgdi32 -lopengl32 -lpython35 -o ComputeShader.pyd

Run:
	$(PY) Test.py
