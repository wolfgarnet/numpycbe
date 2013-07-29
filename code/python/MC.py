import time
import numpy

random=numpy.random.rand
square=numpy.square
add=numpy.add
less_equal=numpy.less_equal
sum=numpy.sum

N=4096*4096
#N=3072*3072
#N=2048*2048
#N=1024*1024


x = numpy.arange(N,dtype="f")
y = numpy.arange(N,dtype="f")



#x,y=(random(s),random(s))
x.fill(random())
y.fill(random())

x = x * x
y = y * y
#(x,y)=(x*x, y*y)

x+=y

z = less_equal(x, 1.0)

start=time.time()
sum(z)*4.0/N

    
stop=time.time()



print "Time: " + str((stop-start))

