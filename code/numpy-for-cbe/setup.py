from distutils.core import setup, Extension
setup(name='numpycbe',ext_modules=[Extension('numpycbe',sources=['blas3test.c'], library_dirs=['/usr/lib/'], libraries=['spe2'])])
