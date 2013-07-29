#include <shaders.h>


// Fillers
#include <randomfiller.h>
unsigned int *SHADER_randomfiller = (unsigned int *)randomfiller;
#include <zerofiller.h>
unsigned int *SHADER_zerofiller = (unsigned int *)zerofiller;

//
#include <arraymultiply.h>
unsigned int *SHADER_arraymultiply = (unsigned int *)arraymultiply;
#include <arraymultiply_scalararray.h>
unsigned int *SHADER_arraymultiply_scalararray = (unsigned int *)arraymultiply_scalararray;
#include <arraymultiply_arrayscalar.h>
unsigned int *SHADER_arraymultiply_arrayscalar = (unsigned int *)arraymultiply_arrayscalar;

#include <arraydivide.h>
unsigned int *SHADER_arraydivide = (unsigned int *)arraydivide;
#include <arrayadd.h>
unsigned int *SHADER_arrayadd = (unsigned int *)arrayadd;
#include <arraysub.h>
unsigned int *SHADER_arraysub = (unsigned int *)arraysub;
#include <arraysum.h>
unsigned int *SHADER_arraysum = (unsigned int *)arraysum;
#include <arraylessequal_arrayscalar.h>
unsigned int *SHADER_arraylessequal_arrayscalar = (unsigned int *)arraylessequal_arrayscalar;

// SOLVE
#ifdef _64BLOCKS
#include <sger_lu64.h>
#include <sger64.h>
#include <strsm_llnu64.h>
#include <strsm_llnu_lu64.h>
#include <strsm_lunn64.h>
#else
#include <sger_lu.h>
#include <sger.h>
#include <strsm_llnu.h>
#include <strsm_llnu_lu.h>
#include <strsm_lunn.h>
#endif
unsigned int *SHADER_sger_lu = (unsigned int *)sger_lu;
unsigned int *SHADER_sger = (unsigned int *)sger;
unsigned int *SHADER_strsm_llnu = (unsigned int *)strsm_llnu;
unsigned int *SHADER_strsm_llnu_lu = (unsigned int *)strsm_llnu_lu;
unsigned int *SHADER_strsm_lunn = (unsigned int *)strsm_lunn;

#ifdef _64BLOCKS
#include <spu_mult_shader.h>
#else
#include <spu_mult_shader_not_unrolled.h>
#endif
unsigned int *SHADER_sgemm_fixed = main2;
unsigned int sgemm_fixed_size = MAIN_SIZE;


#include <arrayslicerow_arrayscalar.h>
unsigned int *SHADER_arrayslicerow_arrayscalar = arrayslicerow_arrayscalar;
unsigned int SHADER_arrayslicerow_arrayscalar_id = 1;


// TEST
unsigned int SHADER_dispatcher_test_id = 2;

