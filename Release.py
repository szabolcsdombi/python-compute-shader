import os, shutil, zipfile, sys

if len(sys.argv) < 2:
	ver = 'beta'

else:
	ver = '-'.join(sys.argv[1:])

if not os.path.isdir('Release'):
	os.mkdir('Release')

package = zipfile.ZipFile('Release\\Release-%s.zip' % ver, 'w')

package.write('LICENSE')
package.write('README.md')

for example in ['Test.py', 'Primes.py']:
	data = open(example, 'rb').read()
	data = data.replace(b'from Bin import ComputeShader', b'from Modules import ComputeShader')
	package.writestr(example, data)

package.write('Bin\\ComputeShader.pyd', 'Modules\\ComputeShader.pyd')
