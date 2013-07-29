import time
import sys
sys.path.append('/home/jens/bin/numpy/lib/python2.5/site-packages/')
from numpy import *
import sys
import os




def sor(A, B, rf, tol=0.005, init_val=0.0):
#    '''itteratively solving for matrix A with solution vector B
#    rf = relaxation factor (rf > 1.0; eg rf = 1.1)
#    tol = tolerance for dh/h
#    init_val = array of initial values to use in the solver
    
#    '''
    print "successive over-relaxation (sor)"
    if init_val == 0.0:
        h = zeros(shape(B), dtype="f")
        hnew = zeros(shape(B), dtype="f")
    else:
        h = init_val
        hnew[:] = h[:]
        
    dmax = 1.0
    n = 0
    
    t1 = t = time.time()
    #while dmax > tol:
    while n < 100:
        n += 1
        for i in range(len(B)):
            #print "add.reduce(multiply(A,h)) ", add.reduce(multiply(A,h))
            hnew[i] = h[i] + (B[i] - add.reduce(multiply(A[i,:],hnew)))/ A[i,i]
            
        #over-relaxation
        dh = subtract(hnew, h)
        hnew = h + multiply(rf,dh)
        dmax = max(abs(divide(subtract(h,hnew), h)))
        #print 'dmax:' + str(dmax)
        #if (time.time() - t) > 10.0:
            #print "Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
            #t = time.time()
            
            ##        print_arr(h, "h")
            ##        print_arr(hnew, "hnew")
        h[:] = hnew[:]
    #print "SOLVED at: Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
    endtime = time.time() - t1
    print "Time = ", endtime, "seconds"
    print "Time per iteration is:  ", endtime/n 
            
    return h




#JMI: This was testdata for the small example that gave correct results:
#A=array((4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,   4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1),float)
#A = reshape(A, (8,8))
#B = array( (1,2,3,4,1,2,3,4),float)
#C = sor(A,B,rf=1.3)
#print 'h is:'
#print C


#Large dataset creation
size = 1024
if len( sys.argv ) > 1:
    size = int(sys.argv[1])

A = array(random.rand(size,size),dtype="f")
B = array(random.rand(size),dtype="f")

print "Size is " + str(size)

C = sor(A,B,rf=1.0)
print 'Solved!'
