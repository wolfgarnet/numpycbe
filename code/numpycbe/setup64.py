from distutils.core import setup, Extension

setup(name='numpycbe',ext_modules=[Extension('numpycbe',sources=['src/shaders.c', 'numpycbe.c','src/libppu.c', 'src/solve.c', 'src/runshader.c'], library_dirs=['/usr/lib/', 'lib/'], libraries=['spe2','ppufhbtool', 'pputimer'], include_dirs=['include/', 'shaders/'], define_macros=[('_64BLOCKS', None)] )])
