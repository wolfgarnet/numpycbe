#include <stdio.h>
#include <Python.h>

void printThis()
{
	printf("Inside printThis\n");
}

static PyMethodDef funcs[] = {
{"printThis",(PyCFunction)printThis,METH_NOARGS,"docstring"},
{NULL}
};

void
inittest(void)
{
	/* omitted */
	PyObject* thismod = Py_InitModule3("test",funcs,"This is a test, bitch.");
}
