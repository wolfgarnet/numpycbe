import time
import numpycbe
X=numpycbe._create(64,64)
Z=numpycbe._create(64,64)
start=time.time()
numpycbe._blas3Mult(X,X,Z)
stop=time.time()
numpycbe._readData(Z)
print 'Time for blas3Mult: ',  (stop-start)
