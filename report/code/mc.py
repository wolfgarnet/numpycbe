import time
import numpycbe

def MC_int_old(s):

    x,y=(numpycbe.create(s,s,1),numpycbe.create(s,s,1))
    x,y=(numpycbe.random(x),numpycbe.random(y))
    (x,y)=(numpycbe.multiply(x,x), numpycbe.multiply(y,y))
    x=numpycbe.add(x,y)
    Y=numpycbe.less_equal(x, 1.0)
    return numpycbe.sum(Y)*4.0/s

start=time.time()
print 'result is:'
print MC_int_old(5)
stop=time.time()
print 'time: ', (stop-start)
