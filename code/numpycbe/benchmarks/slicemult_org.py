import numpy as np
import time
import sys
import os

size = 8

if len( sys.argv ) > 1:
    size = int(sys.argv[1])

a = np.arange(0,size*size, dtype="f").reshape(size,size)
b = np.arange(0,size, dtype="f")
c = np.arange(0,size, dtype="f")

start=time.time()

for i in range(size):
    c = np.multiply(a[i,:],b)

#for i in range(4096):
#    c = np.multiply(a[0,:],b)

stop=time.time()

print "Time taken: " + str(stop-start) + " seconds"
print "Did " + str(size) + " iterations"

if size < 32:
    print c
