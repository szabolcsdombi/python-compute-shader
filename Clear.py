import shutil, os

if os.path.isdir('Bin'):
	shutil.rmtree('Bin')
	
if os.path.isdir('Temp'):
	shutil.rmtree('Temp')
