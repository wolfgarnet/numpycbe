#include "helpers.h"
#include "init.h"

#include <stdarg.h>

//void printf2( char *msg, ... )
//{
//	va_list argp;
//
//	printf( "SPE[%u]: ", SPE_id );
//	va_start(argp, msg);
//	vprintf( msg, argp);
//	va_end(argp);
//	printf("<------\n");
//
//}


void PrintUInt( unsigned int i )
{
	printf( "SPE[%u]: UInt=%u\n", SPE_id, i );
}

void PrintInt( int i )
{
	printf( "SPE[%u]: Int=%i\n", SPE_id, i );
}

void PrintE( long long i )
{
	//printf( "SPE[%u]: Int=%e\n", SPE_id, i );
	printf( "SPE[%u]: Int=%e\n", SPE_id, (double)i );
}

void PrintChar( char c )
{
	printf( "SPE[%u]: Char=%f\n", SPE_id, c );
}




#ifdef _SCI_PRECISION

void PrintFloat( float f )
{
	printf( "SPE[%u]: Float=%e\n", SPE_id, f );
}

#else

void PrintFloat( float f )
{
	printf( "SPE[%u]: Float=%f\n", SPE_id, f );
}

#endif




#ifdef _SCI_PRECISION

void PrintFloat3( float f1, float f2, float f3 )
{
	printf( "SPE[%u]: Float1=%e, Float1=%e, Float1=%e\n", SPE_id, f1, f2, f3 );
}

#else

void PrintFloat3( float f1, float f2, float f3 )
{
	printf( "SPE[%u]: Float1=%f, Float1=%f, Float1=%f\n", SPE_id, f1, f2, f3 );
}

#endif




#ifdef _SCI_PRECISION

void PrintFloatRow( float *row, unsigned int size, unsigned int rowid )
{
	printf( "SPE[%u]: Row=%u : ", SPE_id, rowid );
	unsigned int i;
	for( i = 0 ; i < size ; i++ )
	{
		printf( "%e ", row[i] );
	}
	printf( "\n" );
}

#else

void PrintFloatRow( float *row, unsigned int size, unsigned int rowid )
{
	printf( "SPE[%u]: Row=%u : ", SPE_id, rowid );
	unsigned int i;
	for( i = 0 ; i < size ; i++ )
	{
		printf( "%.2f ", row[i] );
	}
	printf( "\n" );
}

#endif



#ifdef _SCI_PRECISION

void PrintFloat3v( vector float *f1, vector float *f2, vector float *f3 )
{
	printf( "SPE[%u]: Float1=%e,%e,%e,%e, Float1=%e,%e,%e,%e, Float1=%e,%e,%e,%e\n", SPE_id, spu_extract(*f1, 0), spu_extract(*f1, 1), spu_extract(*f1, 2), spu_extract(*f1, 3),
																							 spu_extract(*f2, 0), spu_extract(*f2, 1), spu_extract(*f2, 2), spu_extract(*f2, 3),
																							 spu_extract(*f3, 0), spu_extract(*f3, 1), spu_extract(*f3, 2), spu_extract(*f3, 3) );
}

#else

void PrintFloat3v( vector float *f1, vector float *f2, vector float *f3 )
{
	printf( "SPE[%u]: Float1=%f,%f,%f,%f, Float1=%f,%f,%f,%f, Float1=%f,%f,%f,%f\n", SPE_id, spu_extract(*f1, 0), spu_extract(*f1, 1), spu_extract(*f1, 2), spu_extract(*f1, 3),
																							 spu_extract(*f2, 0), spu_extract(*f2, 1), spu_extract(*f2, 2), spu_extract(*f2, 3),
																							 spu_extract(*f3, 0), spu_extract(*f3, 1), spu_extract(*f3, 2), spu_extract(*f3, 3) );
}

#endif



#ifdef _SCI_PRECISION

void PrintFloatv( vector float *f1 )
{
	printf( "SPE[%u]: Float=%e,%e,%e,%e\n", SPE_id, spu_extract(*f1, 0), spu_extract(*f1, 1), spu_extract(*f1, 2), spu_extract(*f1, 3) );
}

#else


void PrintFloatv( vector float *f1 )
{
	printf( "SPE[%u]: Float=%f,%f,%f,%f\n", SPE_id, spu_extract(*f1, 0), spu_extract(*f1, 1), spu_extract(*f1, 2), spu_extract(*f1, 3) );
}

#endif

void PrintAddr( unsigned int i )
{
	printf( "SPE[%u]: Addr=%#x\n", SPE_id, i );
}

void PrintString( char *s )
{
	printf( "String=%s\n", s );
}

double B2K( unsigned int bytes )
{
	double r = ( bytes / 1024 );
	return r;
}


void SelfCheck()
{
#ifdef __DISPATCHER_TYPE_1
	printf( "Setup for default thesis\n" );
#endif
#ifdef __DISPATCHER_TYPE_2
	printf( "Setup for ideal\n" );
#endif
#ifdef __DISPATCHER_TYPE_3
	printf( "Setup for large blocks\n" );
#endif
#ifdef __DISPATCHER_TYPE_4
	printf( "Setup for small blocks\n" );
#endif
	printf( "---- Memory self check ----\n" );
	printf( "Size of shader = %.1f(%.1f)\n", B2K( __SHADER_SIZE ), B2K( __SHADER_SIZE * __NUMBER_OF_SHADERS ) );
	printf( "Size of blocks = %.1f\n", B2K( __BLOCK_DATA_SIZE ) );
	printf( "In total = %.1f\n", B2K( (__BLOCK_DATA_SIZE) * 2 * (__NUMBER_OF_BLOCKS_IN_MEM) ));
	printf( "Size of meta = %.1f\n", B2K( __META_DATA_SIZE ) );
	printf( "---------------------------\n");
	printf( "In total = %.1f\n", B2K( ((__BLOCK_DATA_SIZE) * 2 * (__NUMBER_OF_BLOCKS_IN_MEM)) + (__META_DATA_SIZE) + (__SHADER_SIZE) ) );
	printf( "This leaves %.1f\n", B2K( 0x40000 - (((__BLOCK_DATA_SIZE) * 2 * (__NUMBER_OF_BLOCKS_IN_MEM)) + (__META_DATA_SIZE) + (__SHADER_SIZE * __NUMBER_OF_SHADERS)) ) );
	printf( "------- End of check ------\n");
}



