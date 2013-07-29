import time
import numpycbe as np


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
RUN(2048)
RUN(3072)
RUN(4096)
