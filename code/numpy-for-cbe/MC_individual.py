import time
import numpycbe

def MC_int_old1(s):

    start=time.time()
    x=numpycbe._initVector()
    stop=time.time()
    print 'time for initVector: ', (stop-start)

    y=numpycbe._initVector()
    
    start=time.time()
    x=numpycbe._random(x)
    stop=time.time()
    print 'time for random: ', (stop-start)
    
    y=numpycbe._random(y)
    
    start=time.time()
    x=numpycbe._mult(x,x)
    stop=time.time()
    print 'time for mult: ', (stop-start)
    
    y=numpycbe._mult(y,y)
    
    start=time.time()
    x=numpycbe._add(x,y)
    stop=time.time()
    print 'time for add: ', (stop-start)
    
    start=time.time()
    Y=numpycbe._lessequal(x, 1.0)
    stop=time.time()
    print 'time for lessequal: ', (stop-start)
    
    start=time.time()
    Z=numpycbe._sum(Y)
    stop=time.time()
    print 'time for sum: ', (stop-start)
    
    return Z*4.0/s


MC_int_old1(5)
