spudispatcher: cdispatcher.o helpers.o libdispatcher.o 
	spu-gcc -L/opt/cell/sysroot/opt/cell/sdk/usr/spu/lib -L/opt/cell/sysroot/usr/lib -Wl,-N -o"sd" cdispatcher.o helpers.o libdispatcher.o -l"mc_rand"

cdispatcher.o: cdispatcher.c
	spu-gcc -D__DISPATCHER_TYPE_1 -I/opt/cell/sysroot/opt/cell/sdk/usr/spu/include -I/opt/ibm/systemsim-cell/include/callthru/spu -I/opt/cell/sysroot/usr/spu/include -O3 -Wall -c -fmessage-length=0 -Winline -Wextra -fno-inline -mtune=cell -mfloat=fast -mdouble=fast -Wno-main -march=cell -mea32 -MMD -MP -o"cdispatcher.o"

helpers.o: helpers.c
	spu-gcc -D__DISPATCHER_TYPE_1 -I/opt/cell/sysroot/opt/cell/sdk/usr/spu/include -I/opt/ibm/systemsim-cell/include/callthru/spu -I/opt/cell/sysroot/usr/spu/include -O3 -Wall -c -fmessage-length=0 -Winline -Wextra -fno-inline -mtune=cell -mfloat=fast -mdouble=fast -Wno-main -march=cell -mea32 -MMD -MP  -o"helpers.o"

libdispatcher.o: libdispatcher.c
	spu-gcc -D__DISPATCHER_TYPE_1 -I/opt/cell/sysroot/opt/cell/sdk/usr/spu/include -I/opt/ibm/systemsim-cell/include/callthru/spu -I/opt/cell/sysroot/usr/spu/include -O3 -Wall -c -fmessage-length=0 -Winline -Wextra -fno-inline -mtune=cell -mfloat=fast -mdouble=fast -Wno-main -march=cell -mea32 -MMD -MP -o"libdispatcher"


