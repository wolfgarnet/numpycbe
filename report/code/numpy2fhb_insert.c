static PyObject *
_array_fromobject(PyObject *ignored, PyObject *args, PyObject *kws)
{

...

finish:
    Py_XDECREF(type);
    if (!ret || (nd=PyArray_NDIM(ret)) >= ndmin)
      {

        numpy2FHB(ret);

        return ret;
      }

...
}