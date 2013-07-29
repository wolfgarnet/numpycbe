from distutils.core import setup, Extension
setup(name='numpycbe',ext_modules=[Extension('numpycbe',sources=['numpycbe.c'], library_dirs=['/usr/lib/'], libraries=['spe2'])])
