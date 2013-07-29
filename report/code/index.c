PyObject *_GetIndexAt( PyObject *self, PyObject *args )
{
	PyObject *result = NULL;
	long a, x, y = 0;

	if ( !PyArg_ParseTuple(args, "ii|i", &a, &x, &y ) ) {
		return Py_None;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	unsigned int col;
	unsigned int row;
	unsigned int block;
	unsigned int index;

	// Vector
	if( obj->nd == 1 ) {
		index = x % ( obj->blockSize * obj->blockSize );
		block = x / ( obj->blockSize * obj->blockSize );
	}
	// Matrix
	else {
		col   = x % obj->blockSize;
		row   = y % obj->blockSize;
		block = y / obj->blockSize * obj->numberOfBlocksXDim + x / obj->blockSize;
		index = row*obj->blockSize+col;
	}
	float r = ((float*)obj->blockData[block])[index];
	result = Py_BuildValue( "f", r );
	return result;
}
