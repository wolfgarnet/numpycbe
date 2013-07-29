import time
import numpycbe as np

np.SetBlockSize(64)

def RUN(size):
    a=np.create(size,size,1)
    b=np.create(size,size,1)

    start=time.time()
    np.Strsm_lunn(a,b)
    stop=time.time()

    print "Time taken: " + str((stop-start)) + " seconds"


RUN(1024)

