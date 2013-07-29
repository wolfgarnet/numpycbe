import time
import numpy as np


#random=numpy.random.random
#square=numpy.square
#add=numpy.add
#less_equal=numpy.less_equal
#sum=numpy.sum



def run(x):
    print str(x) + " x " + str(x)
    N=x*x

    a = np.arange(N,dtype="f")
    a.fill(1)

    # Random
    start=time.time()
    a.fill(np.random.rand())
    stop=time.time()
    print "(rand)Time taken " + str((stop-start)*1000)

    # Mult
    start=time.time()
    a = a * a
    stop=time.time()
    print "(mult)Time taken " + str((stop-start)*1000)

    # Add
    start=time.time()
    a = a + a
    stop=time.time()
    print "(Add)Time taken " + str((stop-start)*1000)

    # Less equal
    start=time.time()
    a = np.less_equal(a,1.0)
    stop=time.time()
    print "(le)Time taken " + str((stop-start)*1000)

    # Sum
    start=time.time()
    a = np.sum(a)
    stop=time.time()
    print "(sum)Time taken " + str((stop-start)*1000)


run(1024)
run(2048)
run(3072)
run(4096)
