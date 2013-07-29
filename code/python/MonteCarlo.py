import time
import numpy


random=numpy.random.random
square=numpy.square
add=numpy.add
less_equal=numpy.less_equal
sum=numpy.sum


def MonteCarlo_num_Mflops(Num_samples):
        #/* 3 flops in x^2+y^2 and 1 flop in random routine */
    
    return  (Num_samples* 4.0)/10E6

def MC_int_old(s):
    x,y=(random(s),random(s))
    (x,y)=(x*x, y*y)
    x+=y
    return sum(less_equal(x, 1.0))*4.0/s

    
def MC_int(s):
    x,y=(random(s),random(s))
    (x,y)=(square(x), square(y))
    x=add(x,y)
    x=less_equal(x, 1.0)
    return sum(x)*4.0/s
    

def MonteCarlo_integrate(Num_samples):
    under_curve = 0

    for count in range(Num_samples):
        (x, y) = (random(),random())
        
        if  (x*x + y*y) <= 1.0:
            under_curve+=1

    return under_curve*4.0/Num_samples




#N=10**7
#N=64*64*64*64
#N=3072*3072
N=2048*2048
#N=1024*1024

print "Number of elements="+str(N)+ ". Number of operations="+str(N*4)
start=time.time()
print MC_int_old(N)
stop=time.time()

print "Time: " + str(stop-start)

print 'Performance ', MonteCarlo_num_Mflops(N)/(stop-start)
start=time.time()
print MC_int(N)
stop=time.time()
print "Time: " + str(stop-start)
print 'Performance ', MonteCarlo_num_Mflops(N)/(stop-start)

