import time
import numpycbe as np


def RUN(size):
    a=np.create(size,size,1)

    print "Summing a (" + str(size) + ", " + str(size) + ") to x"

    start=time.time()

    x = np.sum( a )

    stop=time.time()

    print "Time taken: " + str((stop-start)*1000)

    np.Delete(a)


RUN(1024)
RUN(2048)
RUN(3072)
RUN(4096)
