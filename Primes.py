from Bin import ComputeShader as CS
import struct

cs = CS.NewCS('''
	#version 430

	layout (binding = 1) buffer Input {
		uint number[];
	};

	layout (binding = 2) buffer Output {
		uint count;
		uint prime[];
	};

	layout (local_size_x = 100, local_size_y = 1, local_size_z = 1) in;

	void main() {
		uint target = number[gl_GlobalInvocationID.x];
		bool isPrime = target >= 2;
		for (uint i = 2; isPrime && i <= target / 2; ++i) {
			if (target % i == 0) {
				isPrime = false;
			}
		}
		if (isPrime) {
			uint id = atomicAdd(count, 1);
			prime[id] = target;
		}
	}
''')

ssbo_in = CS.NewSSBO(b''.join(struct.pack('I', i) for i in range(1000)));
ssbo_out = CS.NewSSBO(struct.pack('I', 0) * (1 + 1000));

CS.UseSSBO(ssbo_in, cs, 1)
CS.UseSSBO(ssbo_out, cs, 2)
CS.UseCS(cs, 10)

primes = struct.unpack('I', CS.ReadSSBO(ssbo_out, 4))[0]
print(primes)

for p in struct.unpack('%dI' % primes, CS.ReadSSBO(ssbo_out, 4 * primes, 4)):
	print(p)

CS.DeleteSSBO(ssbo_in)
CS.DeleteSSBO(ssbo_out)
CS.DeleteCS(cs)

