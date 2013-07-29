
#include <runshader.h>

#include <common.h>
#include <common_ppu.h>


// OBS!!!! Det skal ikke være konstante værdier!!!!
inline unsigned int _GetNumberOfSPES( unsigned int blocks )
{
	if( blocks > 12 )
	{
		return 6;
	}
	else
	{
		return (blocks+1)/2;
	}
}

/*
 * Run with one array
 */
void run10( PyArrayObject *pyobj1, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;

	// Determining the number of SPEs
	op1.num_SPES = _GetNumberOfSPES( pyobj1->numberOfBlocks );

	Printf1( "Using %u SPEs\n", op1.num_SPES );

	op1.obj[0]  = pyobj1;

	unsigned int checked = 0;

	unsigned int state = 0, r;
	unsigned int i;

  	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < op1.num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}

/*
 * Run with two arrays
 */
void run20( PyArrayObject *pyobj1, PyArrayObject *pyobj2, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;

	// Determining the number of SPEs
	op1.num_SPES = _GetNumberOfSPES( pyobj1->numberOfBlocks );
	//op1.num_SPES = 1;

	Printf1( "Using %u SPEs\n", op1.num_SPES );

	op1.obj[0]  = pyobj1;
	op1.obj[1]  = pyobj2;

	unsigned int checked = 0;

	unsigned int state = 0, r;
	unsigned int i;

  	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < op1.num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}

/*
 * Run with two arrays and two scalars
 */
void run22( PyArrayObject *A, PyArrayObject *x, PyArrayObject *alpha, PyArrayObject *j, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;

	// Determining the number of SPEs
	op1.num_SPES = _GetNumberOfSPES( A->numberOfBlocks );

	op1.obj[0]  = A;
	op1.obj[1]  = x;
	op1.scalar[0]  = alpha;
	op1.scalar[1]  = j;

	unsigned int checked = 0;

	unsigned int state = 0, r;
	unsigned int i;

  	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < op1.num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}

/*
 * Run with two arrays and one scalar
 */
void run21( PyArrayObject *pyobj1, PyArrayObject *pyobj2, PyArrayObject *scalar, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;

	// Determining the number of SPEs
	op1.num_SPES = _GetNumberOfSPES( pyobj1->numberOfBlocks );
	//op1.num_SPES = 1;

	printf("using %u spes\n",op1.num_SPES);

	op1.obj[0]  = pyobj1;
	op1.obj[1]  = pyobj2;
	op1.scalar[0]  = scalar;

	unsigned int checked = 0;

	unsigned int state = 0, r;
	unsigned int i;

  	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < op1.num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}


/*
 * Run with three arrays and one scalar
 */
void run31( PyArrayObject *pyobj1, PyArrayObject *pyobj2, PyArrayObject *pyobj3, PyArrayObject *scalar, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;

	// Determining the number of SPEs
	op1.num_SPES = _GetNumberOfSPES( pyobj1->numberOfBlocks );

	op1.obj[0]  = pyobj1;
	op1.obj[1]  = pyobj2;
	op1.obj[2]  = pyobj3;
	op1.scalar[0]  = scalar;

	unsigned int checked = 0;

	unsigned int state = 0, r;
	unsigned int i;

  	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < op1.num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}

/*
 * Run with three arrays and one scalar
 */
void run32( PyArrayObject *pyobj1, PyArrayObject *pyobj2, PyArrayObject *pyobj3, PyArrayObject *scalar1, PyArrayObject *scalar2, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;

	// Determining the number of SPEs
	op1.num_SPES = _GetNumberOfSPES( pyobj1->numberOfBlocks );

	op1.obj[0]  = pyobj1;
	op1.obj[1]  = pyobj2;
	op1.obj[2]  = pyobj3;
	op1.scalar[0]  = scalar1;
	op1.scalar[1]  = scalar2;

	unsigned int checked = 0;

	unsigned int state = 0, r;
	unsigned int i;

  	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < op1.num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}


/*
 * Run with three arrays
 */
void run30( PyArrayObject *pyobj1, PyArrayObject *pyobj2, PyArrayObject *pyobj3, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;

	// Determining the number of SPEs
	op1.num_SPES = _GetNumberOfSPES( pyobj1->numberOfBlocks );
	//op1.num_SPES = 1;

	op1.obj[0]  = pyobj1;
	op1.obj[1]  = pyobj2;
	op1.obj[2]  = pyobj3;

	unsigned int checked = 0;

	unsigned int state = 0, r;
	unsigned int i;

  	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < op1.num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}



/*******************************************************
 * Functionality with return values
 *******************************************************/



/*
 * Run with one array, with return value
 */
float run10r( PyArrayObject *pyobj1, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;

	// Determining the number of SPEs
	op1.num_SPES = _GetNumberOfSPES( pyobj1->numberOfBlocks );

	op1.obj[0]  = pyobj1;

	//float *results[6];

	unsigned int checked = 0;

	unsigned int state = 100, r;
	unsigned int i;

  	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
  		//results[i] = (float *)memalign( 128, ( 4 + 127 ) & ~127 );
  		r = (unsigned int)resultFloats[i];

  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
  		spe_in_mbox_write ( speData[i].spe_ctx, &r, 1, SPE_MBOX_ALL_BLOCKING );
	}



  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < op1.num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}

	float r1 = 0.0f;
	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
		r1 += resultFloats[i][0];
		// Cleanup
		// free( results[i] );
	}

	return r1;
}

/*
 *
 *
 *
 */

/*
 * Run with three arrays, no upload
 */
void run30nu( unsigned int shaderindex )
{
	unsigned int checked = 0;

	unsigned int state = 300 + shaderindex, r;
	unsigned int i;

	unsigned int num_SPES = 1;

	printf( "HERE2\n" );

  	for ( i = 0 ; i < num_SPES ; i++ )
	{
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}


/*
 * Run with three arrays, no run
 */
void run30nr( PyArrayObject *pyobj1, PyArrayObject *pyobj2, PyArrayObject *pyobj3, unsigned int shadersize, unsigned int *shader, unsigned int shaderindex )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;

	// Determining the number of SPEs
	//op1.num_SPES = _GetNumberOfSPES( pyobj1->numberOfBlocks );
	op1.num_SPES = 1;

	op1.obj[0]  = pyobj1;
	op1.obj[1]  = pyobj2;
	op1.obj[2]  = pyobj3;

	unsigned int checked = 0;

	unsigned int state = 200 + shaderindex, r;
	unsigned int i;

//	printf( "HERE %u!\n", state );
//	printf( "SIZE=%u, ADDR=%#x\n", op1.shaderSize, op1.EA_shader );

  	for ( i = 0 ; i < op1.num_SPES ; i++ )
	{
  		//printf( "-->%u\n",i );
  		spe_pointer_addr[i][shaderindex] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < op1.num_SPES )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}
