import time
import numpy as np

def MC_int_old1(s,k):

    Z = 0
    x = np.arange(s*s,dtype="f")
    y = np.arange(s*s,dtype="f")
    
    for i in range(0, k):
        x.fill(np.random.rand())
        y.fill(np.random.rand())
        x=x*x
        y=y*y
        x+=y
        y=np.less_equal(x, 1.0)
        Z = Z + np.sum(y)
        print "Z="+str(Z)

    return Z*4.0/(s*s*k)
    

start=time.time()
print MC_int_old1(1024,1)
stop=time.time()

print "Time: " + str(stop-start)    

