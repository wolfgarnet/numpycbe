#ifndef __SHADERS_H

#define __SHADERS_H



typedef unsigned int uint32_t;

unsigned int SHADER_index[4];
unsigned int SHADER_status[100];



unsigned int randomfiller_size;
unsigned int *SHADER_randomfiller;
unsigned int SHADER_randomfiller_id;

unsigned int zerofiller_size;
unsigned int *SHADER_zerofiller;
unsigned int SHADER_zerofiller_id;

unsigned int arraymultiply_size;
unsigned int *SHADER_arraymultiply;
unsigned int SHADER_arraymultiply_id;

unsigned int arraymultiply_scalararray_size;
unsigned int *SHADER_arraymultiply_scalararray;

unsigned int arraymultiply_arrayscalar_size;
unsigned int *SHADER_arraymultiply_arrayscalar;

unsigned int arraydivide_size;
unsigned int *SHADER_arraydivide;

unsigned int arrayadd_size;
unsigned int *SHADER_arrayadd;

unsigned int arraysub_size;
unsigned int *SHADER_arraysub;

unsigned int arraysum_size;
unsigned int *SHADER_arraysum;

unsigned int arraylessequal_arrayscalar_size;
unsigned int *SHADER_arraylessequal_arrayscalar;

unsigned int sger_lu_size;
unsigned int *SHADER_sger_lu;

unsigned int sger_size;
unsigned int *SHADER_sger;

unsigned int strsm_llnu_size;
unsigned int *SHADER_strsm_llnu;

unsigned int strsm_llnu_lu_size;
unsigned int *SHADER_strsm_llnu_lu;

unsigned int strsm_lunn_size;
unsigned int *SHADER_strsm_lunn;

unsigned int sgemm_fixed_size;
unsigned int *SHADER_sgemm_fixed;

unsigned int arrayslicerow_arrayscalar_size;
unsigned int *SHADER_arrayslicerow_arrayscalar;
unsigned int SHADER_arrayslicerow_arrayscalar_id;


// Test
unsigned int SHADER_dispatcher_test_id;

#endif // __SHADERS_H
