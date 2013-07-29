import time
import numpycbe as np

def MC_int_old1(s,k):

    Z = 0
    
    x=np.create(s,s, 1)
    y=np.create(s,s, 1)
    
    for i in range(0, k):
        x=np.random(x)
        y=np.random(y)
        x=np.multiply(x,x)
        y=np.multiply(y,y)
        x=np.add(x,y)
        y=np.less_equal(x, 1.0)
        Z = Z + np.sum(y)
        print "Z="+str(Z)
    return Z*4.0/(s*s*k)

    

start=time.time()
print MC_int_old1(4096,1)
stop=time.time()

print "Time: " + str(stop-start)    

