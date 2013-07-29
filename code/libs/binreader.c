/*
 * binreader.c
 *
 *  Created on: Jan 27, 2009
 *      Author: wolle
 */

#include <stdio.h>
#include <malloc.h>

unsigned int binreader( char *binfile, unsigned int **bin )
{
	FILE *fp;

	fp = fopen( binfile, "r" );

	if( fp == NULL )
	{
		printf( "Unable to open %s\n", binfile );
		return NULL;
	}

    fseek( fp, 0, SEEK_END );

    unsigned int size = ftell( fp );
    size += 16 - size % 16;//JMI:added
    //size /= 4;//JMI:removed

    rewind( fp );

    // Reserve space
    *bin = (unsigned int*)memalign( 128, size );//jmi:removed '* sizeof(unsigned int)'

	// Begin reading:
    unsigned int i, temp;
    for ( i = 0 ; i < size ; i++ )
    {
    	fread( &temp, sizeof( unsigned int ), 1, fp );
    	bin[0][i] = temp;
    	//printf( "%d\n", temp );
    }

    fclose( fp );

    printf( "Done reading file %s\n", binfile );

    return size;
}
