import numpycbe as np
import time
import sys
import os

size = 8

if len( sys.argv ) > 1:
    size = int(sys.argv[1])


start=time.time()

for i in range(size):
    c = np.testDispatcher2()

stop=time.time()

print "Time taken: " + str(stop-start) + " seconds"
print "Did " + str(size) + " iterations"

