from Bin import ComputeShader as CS

info = CS.Info()

print('Info():')
for key in sorted(info):
	print('|', key, info[key])