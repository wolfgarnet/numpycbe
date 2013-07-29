import numpycbe

A=numpycbe._createVector(1,1)
print 'A is:'
numpycbe._readData(A)

print numpycbe.max(A)
