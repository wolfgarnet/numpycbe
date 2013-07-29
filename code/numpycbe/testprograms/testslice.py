import numpycbe

A=numpycbe._createSOR(2,2)
print 'A just after creation'
numpycbe._readData(A)
C=numpycbe._createSOR(2,2)
print 'C just after creation'
numpycbe._readData(C)
numpycbe._sliceRow(A,1)
V=numpycbe._createVector(2,2)
print 'V just after creation'
numpycbe._readData(V)
print 'and A is still:'
numpycbe._readData(A)
numpycbe._sliceMult(A,V,C)

numpycbe._readData(C)
