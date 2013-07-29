import numpycbe as np
import time
import sys
import os

size = 1024

if len( sys.argv ) > 1:
    size = int(sys.argv[1])

np.SetBlockSize(64)

print "Creating two matrices of size " + str(size) + ", " + str(size)

a=np.create(size,size,1)
b=np.create(size,size,1)


start=time.time()
np.solve(a,b)
stop=time.time()

print "Time taken: " + str(stop-start) + " seconds"
