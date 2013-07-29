import numpy as np
import time

def SOLVE(size):
    a=np.ones([size,size], dtype="f")
    b=np.ones([size,size], dtype="f")

    for i in range(size):
        for j in range(size):
            #a[j,i] = i+(j/10000.0)
            a[j,i] = size*size-i*size+j

#    print "Converting to column major"
    
#    a = np.reshape(a, [size,size], order="f")

    print a

    print "Solving Ax=B. " + str(size) + " x " + str(size)

    start=time.time()

    np.linalg.solve(a,b)
            
    stop=time.time()

    print "Time taken: " + str((stop-start))

    print a
    print b

SOLVE(8)

#SOLVE(1024)
#SOLVE(1024)

#SOLVE(2048)
#SOLVE(2048)

#SOLVE(3072)
#SOLVE(3072)

#SOLVE(4096)
#SOLVE(4096)
