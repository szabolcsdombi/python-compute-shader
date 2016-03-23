import os, shutil, zipfile, sys

if len(sys.argv) == 2:
	ver = sys.argv[1]

else:
	ver = 'beta'

if not os.path.isdir('Release'):
	os.mkdir('Release')

package = zipfile.ZipFile('Release\\Release-%s.zip' % ver, 'w')

package.write('LICENSE')
package.write('README.md')

for example in ['Test.py', 'Primes.py']:
	data = open(example, 'rb').read()
	data = data.replace(b'from Bin import ComputeShader', b'from Modules import ComputeShader')
	package.writestr(example, data)

package.write('Bin\\ComputeShader.py', 'Modules\\ComputeShader.py')
package.write('Bin\\ComputeShader32.pyd', 'Modules\\ComputeShader32.pyd')
package.write('Bin\\ComputeShader64.pyd', 'Modules\\ComputeShader64.pyd')
