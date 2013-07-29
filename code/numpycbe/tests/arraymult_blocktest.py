import time
import sys
import os
import numpycbe as np

blocksize = 64

if len( sys.argv ) > 1:
    blocksize = int(sys.argv[1])

#print "A yeah!" + str(blocksize)
np.SetBlockSize(blocksize)
#np.SetBlockSize(4)
#print "WOLLA"

def RUN(size):
    a=np.create(size,size,1)
    b=np.create(size,size,1)

    print "Multiplying a and b (" + str(size) + ", " + str(size) + ")"

    start=time.time()

    x = np.multiply( a, b )

    stop=time.time()

    print "Time taken: " + str((stop-start)*1000)

    np.Delete(a)
    np.Delete(b)


RUN(1024)
