import time
import numpycbe as np


def RUN(size):
    a=np.create(size,size,1)

    print "Random (" + str(size) + ", " + str(size) + ")"

    start=time.time()

    x = np.random( a )

    stop=time.time()

    print "Time taken: " + str(stop-start)

    np.Delete(a)


RUN(1024)
RUN(2048)
RUN(3072)
RUN(4096)
