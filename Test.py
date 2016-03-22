import Bin.ComputeShader as CS
import struct

cs = CS.NewCS('''
	#version 430

	layout (binding = 0) buffer Test {
		uint x[16][16];
		int y;
	};

	layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

	void main() {
		x[gl_GlobalInvocationID.x][gl_GlobalInvocationID.y] = gl_LocalInvocationID.x + gl_LocalInvocationID.y + 1000;
		atomicAdd(y, 1);
	}
''')

ssbo = CS.NewSSBO(b'\x00' * (16 * 16 * 4 + 4));

CS.UseSSBO(ssbo, cs)
CS.UseCS(cs, 2, 2)

for i in struct.unpack('256II', CS.ReadSSBO(ssbo, 16 * 16 * 4 + 4)):
	print(i)

CS.DeleteSSBO(ssbo)
CS.DeleteCS(cs)