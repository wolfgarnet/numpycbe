typedef struct
{
	unsigned int *EA_shader;
	unsigned int shaderSize;
	PyArrayObject *obj[3];
	PyArrayObject *scalar[2];
	unsigned int num_SPES;
} Operation_t __attribute__((aligned (16)));
