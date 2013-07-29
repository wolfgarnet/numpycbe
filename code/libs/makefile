fhbtool: fhbtool.c
	ppu-gcc -I/opt/cell/sysroot/opt/cell/sdk/usr/include -I../common -O3 -Wall -c -fmessage-length=0 -Winline -Wextra -fno-inline -m32 -mabi=altivec -maltivec -MMD -MP fhbtool.c -otest.o

	ppu-ar -qcs "test.a" test.o