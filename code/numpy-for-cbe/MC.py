import time
import numpycbe

def MC_int_old(s):

    x,y=(numpycbe._initVector(),numpycbe._initVector())
    #print '1'    
    x,y=(numpycbe._random(x),numpycbe._random(y))
    #print '2'
    (x,y)=(numpycbe._mult(x,x), numpycbe._mult(y,y))
    #print '3'
    x=numpycbe._add(x,y)
    #print '4'
    Y=numpycbe._lessequal(x, 1.0)
    #print '5'    
    return numpycbe._sum(Y)*4.0/s

start=time.time()
print MC_int_old(5)
stop=time.time()
print 'time: ', (stop-start)
