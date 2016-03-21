import os, shutil

if not os.path.isdir('Bin'):
	os.mkdir('Bin')

if not os.path.isdir('Temp'):
	os.mkdir('Temp')

shutil.copyfile('Source\\ComputeShader.py', 'Bin\\ComputeShader.py')
