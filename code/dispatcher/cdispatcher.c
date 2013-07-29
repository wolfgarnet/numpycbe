/*
 * dispatcher.c
 *
 *  Created on: Mar 23, 2009
 *      Author: wolle
 */


//#include "common_spu.h"
#include "init.h"
#include "../common/common_spu.h"
#include "../common/common.h"
#include "helpers.h"

#include <malloc_align.h>
#include "libdispatcher.h"
#include <mc_rand.h>

// PROFILING
//#include <profile.h>



int main( unsigned long long id  __attribute__ ((unused)), unsigned long long argv )
{
	unsigned int i, EA = argv;
	/*
	 * Allocate memory
	 *
	 */

	// Info
	LS_ShaderInfo shaderinfo;

	// Memory for the shader
	unsigned int *shader[__NUMBER_OF_SHADERS];
	for( i = 0 ; i < __NUMBER_OF_SHADERS ; i++ )
	{
		shader[i] = (unsigned int *)_malloc_align( __SHADER_SIZE, 7 );
	}

	// Memory for the blocks
	char *blocks[(__NUMBER_OF_BLOCKS_IN_MEM) * 2];
	for( i = 0 ; i < __NUMBER_OF_BLOCKS_IN_MEM * 2 ; i++ )
	{
		blocks[i] = (char *)_malloc_align( __BLOCK_DATA_SIZE, 7 );
		shaderinfo.LS_blockDataArea[i] = blocks[i];
	}

	// Memory for metadata
	char *meta = (char *)_malloc_align( __META_DATA_SIZE, 7 );
	shaderinfo.LS_shaderMemory = meta;

	Operation_t myop[__NUMBER_OF_SHADERS];
	unsigned int PPE_addr[__NUMBER_OF_SHADERS] __attribute__((aligned(16)));
	unsigned int EA_result[__NUMBER_OF_SHADERS];

	// Getting SPE id
	SPE_id   = mb_getmbox( );
	// SELF CHECK
	//if ( SPE_id == 0 ) SelfCheck();
	//printf( "SPE[%u]: SPE_* is at %#x\n", SPE_id, EA );

	//printf( "[SPE(%u)]Check!!!\n", SPE_id );

	// SPU program fragment prototype
	void (*run)( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs ) = NULL;
	void (*runr)( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs ) = NULL;

	// Getting SPE id
	unsigned int seed = mb_getmbox( );
	// Init random
	mc_rand_ks_init( seed );

	// Common Functions
	Functions_t funcs;
	funcs.printuint      = PrintUInt;
	funcs.printint       = PrintInt;
	funcs.printe         = PrintE;
	funcs.printchar      = PrintChar;
	funcs.printfloat     = PrintFloat;
	funcs.printaddr      = PrintAddr;
	funcs.printstr       = PrintString;
	funcs.printfloat3    = PrintFloat3;
	funcs.printfloat3v   = PrintFloat3v;
	funcs.printfloatv    = PrintFloatv;
	funcs.printfloatrow  = PrintFloatRow;

	//funcs.printf         = printf2;

	funcs.rand_0_to_1_f  = mc_rand_ks_0_to_1_f4;
	//funcs.rand_0_to_1_fm = mc_rand_ks_0_to_1_array_f4;

	unsigned int running = 1, task = 0;

//	prof_clear();
//	prof_start();

	unsigned int idt;

	// THA LOOP
	while( running )
	{
		task = mb_getmbox( );

		Printf1( "[SPE(%u)]Got state %u\n", SPE_id, task );

		// Transfer operation and run shader with no return value
		// 0 -> 99
		if( task < 100 )
		{
			idt = task;

			CHECKBOUNDS( idt );

			Printf1( "SPE[%u]: Get shader for slot %u\n", SPE_id, idt );

			GetSPEAddr( EA, PPE_addr );
			GetOperation( PPE_addr[idt], &myop[idt] );
			GetShader( (unsigned int)myop[idt].EA_shader, myop[idt].shaderSize, shader[idt] );
			Printf1( "SPE[%u]: Shader recieved for slot\n", SPE_id );
			run = (void *)shader[0];

			Printf1( "SPE[%u]: --- Running shader! ----------\n", SPE_id );
			run( SPE_id, &shaderinfo, &myop[idt], &funcs );
			Printf1( "SPE[%u]: --- End of shader! -----------\n", SPE_id );

			spu_writech( SPU_WrOutMbox, 1 );
		}

		// Transfer operation and run shader WITH return value
		// 100 -> 199
		else if( task < 200 )
		{
			idt = task - 100;

			CHECKBOUNDS( idt );

			Printf1( "SPE[%u]: Get shader with return value for slot %u\n", SPE_id, idt );

			EA_result[idt] = mb_getmbox( );
			GetSPEAddr( EA, PPE_addr );
			GetOperation( PPE_addr[idt], &myop[idt] );
			GetShader( (unsigned int)myop[idt].EA_shader, myop[idt].shaderSize, shader[idt] );
			runr = (void *)shader[idt];

			Printf1( "SPE[%u]: --- Running shader! ----------\n", SPE_id );
			runr( SPE_id, EA_result[idt], &shaderinfo, &myop[idt], &funcs );

			Printf1( "SPE[%u]: --- End of shader! -----------\n", SPE_id );

			spu_writech( SPU_WrOutMbox, 1 );
		}


		// Transfer operation and shader WITHOUT return value
		// 200 -> 299
		else if( task < 300 )
		{
			idt = task - 200;

			CHECKBOUNDS( idt );

			Printf1( "SPE[%u]: Get shader for slot %u\n", SPE_id, idt );
			GetSPEAddr( EA, PPE_addr );
			GetOperation( PPE_addr[idt], &myop[idt] );
			GetShader( (unsigned int)myop[idt].EA_shader, myop[idt].shaderSize, shader[idt] );
			spu_writech( SPU_WrOutMbox, 1 );
		}

		// Run shader with no return value
		// 300 -> 399
		else if( task < 400 )
		{
			idt = task - 300;

			CHECKBOUNDS( idt );

			Printf1( "SPE[%u]: Run shader from slot %u\n", SPE_id, idt );
			run = (void *)shader[idt];
			run( SPE_id, &shaderinfo, &myop[idt], &funcs );
			spu_writech( SPU_WrOutMbox, 1 );
		}

		// Transfer operation and shader WITH return value
		// 400 -> 499
		else if( task < 500 )
		{
			idt = task - 400;

			CHECKBOUNDS( idt );

			Printf1( "SPE[%u]: Get shader with return value for slot %u\n", SPE_id, idt );
			EA_result[idt] = mb_getmbox( );
			GetSPEAddr( EA, PPE_addr );
			GetOperation( PPE_addr[idt], &myop[idt] );
			GetShader( (unsigned int)myop[idt].EA_shader, myop[idt].shaderSize, shader[idt] );
			Printf1( "SPE[%u]: Done getting operation %u\n", SPE_id, idt );
			spu_writech( SPU_WrOutMbox, 1 );
		}

		// Run shader WITH return value
		// 500 -> 599
		else if( task < 600 )
		{
			idt = task - 500;

			CHECKBOUNDS( idt );

			Printf1( "SPE[%u]: Run shader with return value from slot %u\n", SPE_id, idt );
			runr = (void *)shader[0];
			runr( SPE_id, EA_result[idt], &shaderinfo, &myop[0], &funcs );
			spu_writech( SPU_WrOutMbox, 1 );
		}

		// Sanity check!!!
		else if( task == 1000 )
		{
			spu_writech( SPU_WrOutMbox, 123 );
		}

		// DEFAULT
		else
		{
			Printf1( "[SPE(%u)]No such instruction, quitting\n", SPE_id );
			running = 0;
		}





//		switch( task )
//		{
//		case 0: // QUIT
//			Printf1( "[SPE(%u)]Quitting\n", SPE_id );
//
//			running = 0;
//			break;
//
//			/*
//			 * Get an operation with no return value
//			 *
//			 */
//		case 1: // Get operation and run
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[0], &myop[0] );
//			GetShader( (unsigned int)myop[0].EA_shader, myop[0].shaderSize, shader[0] );
//			run = (void *)shader[0];
//
//			Printf1( "SPE[%u]: --- Running shader! ----------\n", SPE_id );
//			run( SPE_id, &shaderinfo, &myop[0], &funcs );
//			Printf1( "SPE[%u]: --- End of shader! -----------\n", SPE_id );
//
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//			/*
//			 * Get an operation with a return value
//			 *
//			 */
//		case 2: // Get operation with return value and run
//			EA_result = mb_getmbox( );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[0], &myop[0] );
//			GetShader( (unsigned int)myop[0].EA_shader, myop[0].shaderSize, shader[0] );
//			runr = (void *)shader[0];
//
//			Printf1( "SPE[%u]: --- Running shader! ----------\n", SPE_id );
//			runr( SPE_id, EA_result, &shaderinfo, &myop[0], &funcs );
//
//			Printf1( "SPE[%u]: --- End of shader! -----------\n", SPE_id );
//
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//		case 3: // Get operation without return value NO RUN!
//
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[0], &myop[0] );
//			GetShader( (unsigned int)myop[0].EA_shader, myop[0].shaderSize, shader[0] );
//			run = (void *)shader[0];
//
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//
//		case 4: // Get operation  with return value NO RUN!
//
//			EA_result = mb_getmbox( );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[0], &myop[0] );
//			GetShader( (unsigned int)myop[0].EA_shader, myop[0].shaderSize, shader[0] );
//			runr = (void *)shader[0];
//
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//		case 5: // RUN!!!! with _NO_ return value
//			Printf1( "SPE[%u]: --- Running shader! ----------\n", SPE_id );
//
//			run( SPE_id, &shaderinfo, &myop[0], &funcs );
//			Printf1( "SPE[%u]: --- End of shader! -----------\n", SPE_id );
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//		case 6: // RUN!!!! with return value
//
//			Printf1( "SPE[%u]: --- Running shader! ----------\n", SPE_id );
//
//			runr( SPE_id, EA_result, &shaderinfo, &myop[0], &funcs );
//
//			Printf1( "SPE[%u]: --- End of shader! -----------\n", SPE_id );
//
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//		/*
//		 * Operations with no return values
//		 */
//		case 100: // Get a shader for slot 0
//			Printf1( "SPE[%u]: Get shader for slot 0\n", SPE_id );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[0], &myop[0] );
//			GetShader( (unsigned int)myop[0].EA_shader, myop[0].shaderSize, shader[0] );
//			break;
//
//		case 101: // Get a shader for slot 1
//			Printf1( "SPE[%u]: Get shader for slot 1\n", SPE_id );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[1], &myop[1] );
//			GetShader( (unsigned int)myop[1].EA_shader, myop[1].shaderSize, shader[1] );
//			break;
//
//		case 102: // Get a shader for slot 2
//			Printf1( "SPE[%u]: Get shader for slot 2\n", SPE_id );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[2], &myop[2] );
//			GetShader( (unsigned int)myop[2].EA_shader, myop[2].shaderSize, shader[2] );
//			break;
//
//		case 103: // Get a shader for slot 3
//			Printf1( "SPE[%u]: Get shader for slot 3\n", SPE_id );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[3], &myop[3] );
//			GetShader( (unsigned int)myop[3].EA_shader, myop[3].shaderSize, shader[3] );
//			break;
//
//		case 110: // Run slot 0
//			Printf1( "SPE[%u]: Run shader from slot 0\n", SPE_id );
//			run = (void *)shader[0];
//			run( SPE_id, &shaderinfo, &myop[0], &funcs );
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//		case 111: // Run slot 1
//			Printf1( "SPE[%u]: Run shader from slot 1\n", SPE_id );
//			run = (void *)shader[1];
//			run( SPE_id, &shaderinfo, &myop[1], &funcs );
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//		case 112: // Run slot 2
//			Printf1( "SPE[%u]: Run shader from slot 2\n", SPE_id );
//			run = (void *)shader[2];
//			run( SPE_id, &shaderinfo, &myop[2], &funcs );
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//		case 113: // Run slot 3
//			Printf1( "SPE[%u]: Run shader from slot 3\n", SPE_id );
//			run = (void *)shader[3];
//			Printf1( "SPE[%u]: Run shader from slot 3\n", SPE_id );
//			run( SPE_id, &shaderinfo, &myop[3], &funcs );
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//			/*
//			 * Update operations
//			 */
//
//		case 120: // Update operation for slot 0
//			Printf1( "SPE[%u]: Update operation for slot 0\n", SPE_id );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[0], &myop[0] );
//			break;
//
//		case 121: // Update operation for slot 1
//			Printf1( "SPE[%u]: Update operation for slot 1\n", SPE_id );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[1], &myop[1] );
//			break;
//
//		case 122: // Update operation for slot 2
//			Printf1( "SPE[%u]: Update operation for slot 2\n", SPE_id );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[2], &myop[2] );
//			break;
//
//		case 123: // Update operation for slot 3
//			Printf1( "SPE[%u]: Update operation for slot 3\n", SPE_id );
//			GetSPEAddr( EA, PPE_addr );
//			GetOperation( PPE_addr[3], &myop[3] );
//			break;
//
//		/*
//		 * Operations with return values
//		 */
//		case 200: // Get a shader for slot 0
//			Printf1( "SPE[%u]: Run shader with return value from slot 0\n", SPE_id );
//			EA_result = mb_getmbox( );
//			GetOperation( PPE_addr[0], &myop[0] );
//			GetShader( (unsigned int)myop[0].EA_shader, myop[0].shaderSize, shader[0] );
//			break;
//
//		case 201: // Get a shader for slot 1
//			Printf1( "SPE[%u]: Run shader with return value from slot 1\n", SPE_id );
//			EA_result = mb_getmbox( );
//			GetOperation( PPE_addr[0], &myop[1] );
//			GetShader( (unsigned int)myop[1].EA_shader, myop[1].shaderSize, shader[1] );
//			break;
//
//		case 210: // Run slot 0
//			runr = (void *)shader[0];
//			runr( SPE_id, EA_result, &shaderinfo, &myop[0], &funcs );
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//		case 211: // Run slot 1
//			runr = (void *)shader[1];
//			runr( SPE_id, EA_result, &shaderinfo, &myop[1], &funcs );
//			spu_writech( SPU_WrOutMbox, 1 );
//			break;
//
//		case 1000: // Sanity check
//			spu_writech( SPU_WrOutMbox, 123 );
//			break;
//
//
//		default:
//			Printf1( "[SPE(%u)]No such instruction, quitting\n", SPE_id );
//			running = 0;
//		}
	}

	//prof_stop();

	return 1;
}

