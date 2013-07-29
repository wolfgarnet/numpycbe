#ifndef INIT_H_
#define INIT_H_

#define __LINK_COMMON_FILES

#define __CHECK_BOUNDS

#ifndef __DISPATCHER_SETUP
#define __DISPATCHER_TYPE_1
#endif

// The default setup for this thesis
#ifdef __DISPATCHER_TYPE_1

#define __BLOCKSIZE 64
#define __BLOCKSIZE_SQUARED __BLOCKSIZE * __BLOCKSIZE
#define __BLOCKSIZE_FOURTH __BLOCKSIZE / 4
#define __BLOCKSIZE_SQUARED_FOURTH __BLOCKSIZE_SQUARED / 4

#define __NUMBER_OF_BLOCKS_IN_MEM 3
#define __NUMBER_OF_SHADERS 4
#define __PROG_GET_MAX_SIZE 0x4000

#define __BLOCK_DATA_SIZE 0x4000 	// 32K * 3 =  96K
#define __META_DATA_SIZE 0x16000 	//            72K
#define __SHADER_SIZE 0x3000 		//            12K

#endif

// The ideal setup, with 32K blocks
// Block size: 88 x 88
// Shaders   : 1
#ifdef __DISPATCHER_TYPE_2

#define __BLOCKSIZE 64
#define __BLOCKSIZE_SQUARED __BLOCKSIZE * __BLOCKSIZE
#define __BLOCKSIZE_FOURTH __BLOCKSIZE / 4
#define __BLOCKSIZE_SQUARED_FOURTH __BLOCKSIZE_SQUARED / 4

#define __NUMBER_OF_BLOCKS_IN_MEM 3
#define __NUMBER_OF_SHADERS 1
#define __PROG_GET_MAX_SIZE 0x4000

#define __BLOCK_DATA_SIZE 0x8000 	// 64K * 3 = 192K
#define __META_DATA_SIZE 0x8000 	//            32K
#define __SHADER_SIZE 0x3000 		//            12K

#endif

// Testing setup, for large blocks
// Block size: 96 x 96
// Shaders   : 1
#ifdef __DISPATCHER_TYPE_3

#define __BLOCKSIZE 64
#define __BLOCKSIZE_SQUARED __BLOCKSIZE * __BLOCKSIZE
#define __BLOCKSIZE_FOURTH __BLOCKSIZE / 4
#define __BLOCKSIZE_SQUARED_FOURTH __BLOCKSIZE_SQUARED / 4

#define __NUMBER_OF_BLOCKS_IN_MEM 3
#define __NUMBER_OF_SHADERS 1
#define __PROG_GET_MAX_SIZE 0x4000

#define __BLOCK_DATA_SIZE 0x9000 	// 36K * 6 = 216K
#define __META_DATA_SIZE 0x4000 	//            32K
#define __SHADER_SIZE 0x3000 		//            12K

#endif

// Testing setup, for small blocks
// Block size: 32 x 32
// Shaders   : 1
#ifdef __DISPATCHER_TYPE_4

#define __BLOCKSIZE 64
#define __BLOCKSIZE_SQUARED __BLOCKSIZE * __BLOCKSIZE
#define __BLOCKSIZE_FOURTH __BLOCKSIZE / 4
#define __BLOCKSIZE_SQUARED_FOURTH __BLOCKSIZE_SQUARED / 4

#define __NUMBER_OF_BLOCKS_IN_MEM 3
#define __NUMBER_OF_SHADERS 1
#define __PROG_GET_MAX_SIZE 0x4000

#define __BLOCK_DATA_SIZE 0x1000 	// 4K * 6 = 24K
#define __META_DATA_SIZE 0x30000 	//            32K
#define __SHADER_SIZE 0x3000 		//            12K

#endif


#define CALCBOUNDS( idt ) (__NUMBER_OF_SHADERS) - idt

#ifdef __CHECK_BOUNDS
#define CHECKBOUNDS( idt ) if(!(CALCBOUNDS(idt))){printf( "FAILED(%u=%u)!\n", idt, CALCBOUNDS(idt) );return(0);}
#else
#define CHECKBOUNDS( idt )
#endif





#endif // INIT_H_
