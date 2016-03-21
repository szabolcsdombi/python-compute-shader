import platform

arch, system = platform.architecture()

if arch == '32bit':
	from .ComputeShader32 import *

if arch == '64bit':
	from .ComputeShader64 import *
