static void *
PyArray_GetPtr(PyArrayObject *obj, register intp* ind)
{
    register int n = obj->nd;
    register intp *strides = obj->strides;
    register char *dptr = obj->data;

    while (n--) dptr += (*strides++) * (*ind++);

    return (void *)dptr;
}
