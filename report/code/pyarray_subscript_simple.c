static PyObject *
array_subscript_simple(PyArrayObject *self, PyObject *op)
{
    intp dimensions[MAX_DIMS], strides[MAX_DIMS];
    intp offset;
    int nd;
    PyArrayObject *other;
    intp value;

    value = PyArray_PyIntAsIntp(op);
    if (!PyErr_Occurred()) {
        return array_big_item(self, value);
    }
    PyErr_Clear();

    /* Standard (view-based) Indexing */
    if ((nd = parse_index(self, op, dimensions, strides, &offset))
        == -1) return NULL;

    /* This will only work if new array will be a view */
    Py_INCREF(self->descr);
    if ((other = (PyArrayObject *)                                  \
         PyArray_NewFromDescr(self->ob_type, self->descr,
                              nd, dimensions,
                              strides, self->data+offset,
                              self->flags,
                              (PyObject *)self)) == NULL)
        return NULL;

    other->base = (PyObject *)self;
    Py_INCREF(self);

    PyArray_UpdateFlags(other, UPDATE_ALL);

    return (PyObject *)other;
}