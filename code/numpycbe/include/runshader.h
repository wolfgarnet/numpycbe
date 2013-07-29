#ifndef RUN_SHADER_H_
#define RUN_SHADER_H_

#include <common.h>

// MISC

inline unsigned int _GetNumberOfSPES( unsigned int blocks );

// No return values
void run10( PyArrayObject *, unsigned int, unsigned int * );

void run20( PyArrayObject *, PyArrayObject *, unsigned int , unsigned int * );
void run21( PyArrayObject *, PyArrayObject *, PyArrayObject *, unsigned int , unsigned int * );
void run22( PyArrayObject *, PyArrayObject *, PyArrayObject *, PyArrayObject *, unsigned int, unsigned int * );

void run30( PyArrayObject *, PyArrayObject *, PyArrayObject *, unsigned int shadersize, unsigned int *shader );
void run31( PyArrayObject *, PyArrayObject *, PyArrayObject *, PyArrayObject *, unsigned int, unsigned int * );
void run32( PyArrayObject *, PyArrayObject *, PyArrayObject *, PyArrayObject *, PyArrayObject *, unsigned int, unsigned int * );

// With return values
float run10r( PyArrayObject *, unsigned int, unsigned int * );



void run30nr( PyArrayObject *, PyArrayObject *, PyArrayObject *, unsigned int, unsigned int *, unsigned int );
void run30nu( unsigned int );

#endif // RUN_SHADER_H_
