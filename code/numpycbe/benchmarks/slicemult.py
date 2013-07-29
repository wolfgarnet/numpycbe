import numpycbe as np
import time
import sys
import os

size = 8

if len( sys.argv ) > 1:
    size = int(sys.argv[1])

np.SetBlockSize(4)

a = np.create(size,size,2)
b = np.create(1,size,2)
c = np.create(1,size,1)

start=time.time()

for i in range(size):
    np._sliceMult(np._sliceRow(a,i),b,c)

stop=time.time()

print "Time taken: " + str(stop-start) + " seconds"
print "Did " + str(size) + " iterations"

if size < 32:
    np.Print(c)
