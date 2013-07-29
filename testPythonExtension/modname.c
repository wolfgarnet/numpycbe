#include <Python.h>
#include <stdio.h>

void testFunction(unsigned int a)
{
  printf("testFunction is called with unsigned int: %d\n",a);
}

    PyObject *MyCommand(PyObject *self, PyObject *args)
	{
      PyObject *result = NULL;
        long a, b;

        if (PyArg_ParseTuple(args, "ii", &a, &b)) {
          result = Py_BuildValue("i", a + b);
        } /* otherwise there is an error,
           * the exception already raised by PyArg_ParseTuple, and NULL is
           * returned.
           */

	//Call something interesting
	testFunction(5);

        return result;
      }

    PyMethodDef methods[] = {
      {"add", MyCommand, METH_VARARGS},
      {NULL, NULL},
    };

    void initmodname()
      {
        (void)Py_InitModule("modname", methods);
      }

