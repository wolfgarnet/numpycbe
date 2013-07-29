1: spu-gcc -O3 -c funcs_uint.c -o funcs_uint.o
2: spu-objdump -D funcs_uint.o > funcs_uint.o.dump
3: python oparser.py funcs_uint.o.dump
