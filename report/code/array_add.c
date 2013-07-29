static PyObject *
array_add(PyArrayObject *m1, PyObject *m2)
{
    return PyArray_GenericBinaryFunction(m1, m2, n_ops.add);
}