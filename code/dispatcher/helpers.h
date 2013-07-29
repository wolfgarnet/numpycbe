#ifndef HELPERS_H_
#define HELPERS_H_

#include "../common/common_spu.h"
#include "../common/common.h"

#define _SCI_PRECISION

//void printf2( char *msg, ... );

void PrintUInt( unsigned int i );

void PrintInt( int i );

void PrintE( long long i );

void PrintChar( char c );

void PrintFloat( float f );
void PrintFloatS( float f );

void PrintFloat3( float f1, float f2, float f3 );

void PrintFloatRow( float *row, unsigned int size, unsigned int rowid );

void PrintFloat3v( vector float *f1, vector float *f2, vector float *f3 );
void PrintFloat3vS( vector float *f1, vector float *f2, vector float *f3 );

void PrintFloatv( vector float *f1 );

void PrintAddr( unsigned int i );

void PrintString( char *s );

double B2K( unsigned int bytes );

void SelfCheck();

#endif // HELPERS_H_
