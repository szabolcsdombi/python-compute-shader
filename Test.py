import Bin.ComputeShader as CS
import struct

cs = CS.NewCS('''
	#version 430

	buffer Test {
		uint x[16][16];
	};

	layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

	void main() {
		x[gl_GlobalInvocationID.x][gl_GlobalInvocationID.y] = gl_LocalInvocationID.x + gl_LocalInvocationID.y + 1000;
	}
''')

ssbo = CS.NewSSBO(b'\x00' * 16 * 16 * 4);

CS.UseSSBO(ssbo, cs, 'Test')
CS.UseCS(cs, 2, 2)

for i in struct.unpack('256I', CS.ReadSSBO(ssbo, 16 * 16 * 4)):
	print(i)

CS.DeleteSSBO(ssbo)
CS.DeleteCS(cs)