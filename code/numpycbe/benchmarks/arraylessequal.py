import time
import numpycbe as np

np.SetBlockSize(64)

def RUN(size):
    a=np.arange(0,size*size,1)

    print "Less equal (" + str(size) + ", " + str(size) + ")"

    start=time.time()

    x = np.less_equal( a, 50 )

    stop=time.time()

    print "Time taken: " + str(stop-start)

    np.Delete(a)


RUN(1024)
RUN(2048)
RUN(3072)
RUN(4096)
