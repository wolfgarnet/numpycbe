/*
 * libdispatcher.c
 *
 *  Created on: Mar 23, 2009
 *      Author: wolle
 */

#include "libdispatcher.h"


unsigned int mb_getmbox( )
{
	do {
	     /*
	      * Do other useful work while waiting.
	      */
	} while ( !spu_readchcnt( SPU_RdInMbox ) );
	return spu_readch( SPU_RdInMbox );
}

void GetSPEAddr( unsigned int ea, unsigned int *PPE_addr )
{

//#ifdef _DEBUG
//#if _DBGLVL > 1
//	printf( "Getting SPE address @ %#x to %#x\n", ea, (unsigned int)PPE_addr );
//#endif
//#endif
	Printf1( "SPE[%u]: Getting SPE address @ %#x to %#x\n", SPE_id, ea, (unsigned int)PPE_addr );
	// Get STRUCTURE
	spu_mfcdma32( PPE_addr, ea, 16, 30, MFC_GET_CMD );
	spu_writech( MFC_WrTagMask, 1 << 30 );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

void GetOperation( unsigned int ea, Operation_t *data )
{

//#ifdef _DEBUG
//#if _DBGLVL > 1
//	printf( "Getting operation @ %#x to %#x(%u)\n", ea, (unsigned int)data, sizeof( Operation_t ) );
//#endif
//#endif
	Printf1( "SPE[%u]: Getting operation @ %#x to %#x(%u)\n", SPE_id, ea, (unsigned int)data, sizeof( Operation_t ) );
	// Get STRUCTURE
	spu_mfcdma32( data, ea, 32, 30, MFC_GET_CMD );
	// Waiting
	spu_writech( MFC_WrTagMask, 1 << 30 );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );
//	printf( "---->%#x\n", (unsigned int)data->EA_shader );
//	printf( "---->%#x\n", (unsigned int)data->shaderSize );
//	printf( "---->%#x\n", (unsigned int)data->obj[0] );
//	printf( "---->%#x\n", (unsigned int)data->obj[1] );
//	printf( "---->%#x\n", (unsigned int)data->obj[2] );
//	printf( "---->%#x\n", (unsigned int)data->scalars[0] );
//	printf( "---->%#x\n", (unsigned int)data->scalars[1] );
//	printf( "---->%#x\n", (unsigned int)data->scalars[2] );
}

void GetShader( unsigned int EA, unsigned int size, unsigned int *shader )
{
//#ifdef _DEBUG
//#if _DBGLVL > 1
//	printf( "Getting shader @ %#x to %#x(%u)\n", EA, (unsigned int)shader, size	 );
//	printf( "Shader size is %u\n", size );
//#endif
//#endif
	Printf1( "SPE[%u]: Getting shader @ %#x to %#x(%u)\n", SPE_id, EA, (unsigned int)shader, size	 );
	// Get shader
	size = ( size + 15 ) &~ 15;

	spu_mfcdma32( shader, EA, size, 29, MFC_GET_CMD );
	spu_writech( MFC_WrTagMask, 1 << 29 );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

