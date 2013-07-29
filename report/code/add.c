PyObject *_add(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  long a;
  long b;

  if ( PyArg_ParseTuple( args, "ii", &a, &b ) ) 
  {
    result = Py_BuildValue( "i", a );
  } 

  run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)a, arrayadd_size, SHADER_arrayadd );

  return result;

}
