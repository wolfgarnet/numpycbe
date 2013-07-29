import time
import numpycbe as np

np.SetBlockSize(64)

def RUN(size):
    a=np.create(size,size,1)
    b=np.create(size,1,1)
    c=np.create(size,1,1)

    start=time.time()
    np.Sger(a,b,c,1)
    stop=time.time()

    print "Time taken: " + str((stop-start)) + " seconds"


RUN(1024)

