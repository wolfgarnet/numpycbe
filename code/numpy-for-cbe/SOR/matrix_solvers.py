from numpy import *
import time

def jacobi(A, B, tol=0.005, init_val=0.0):
    '''itteratively solving for matrix A with solution vector B
       tol = tolerance for dh/h
       init_val = array of initial values to use in the solver
    '''
    print "jacobi solver"
    if init_val == 0.0:
        h = zeros(shape(B), float)
    else:
        h = init_val
    h = zeros(shape(B), float)
    dmax = 1.0
    n = 0

    t1 = t = time.time()
    while dmax > tol:
        n += 1
        #print "  Iteration = ", n
        hnew = h+ divide(subtract(B, add.reduce(multiply(A,h),1)), diag(A))
##        print_arr(multiply(A,h), "multiply(A,h)")
##        print_arr(add.reduce(multiply(A,h),1), "add.reduce(multiply(A,h))")
##        print_arr(subtract(B, add.reduce(multiply(A,h),1)), "subtract(B, add.reduce(multiply(A,h)))")
##        print_arr(hnew, "hnew")
        dmax = max(abs(divide(subtract(h,hnew), h)))
        #print "   dmax = ", dmax, ": tol = ", tol
        #print "subtract", subtract(h,hnew)
        if (time.time() - t) > 2.0:
            print "Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
            t = time.time()
        h = hnew
##        print n, dmax,  h
##        print " "
    #print_arr(h, "h")
    print "SOLVED at: Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
    print "                           Time = ", time.time() - t1, "seconds"

    return h


def gauss_seidel(A, B, tol=0.005, init_val=0.0):
    '''itteratively solving for matrix A with solution vector B
       tol = tolerance for dh/h
       init_val = array of initial values to use in the solver
    '''
    print "gauss_siedel"
    if init_val == 0.0:
        h = zeros(shape(B), float)
        hnew = zeros(shape(B), float)
    else:
        h = init_val
        hnew[:] = h[:]
    dmax = 1.0
    n = 0

    t1 = t = time.time()
    while dmax > tol:
        n += 1
        #print "  Iteration = ", n
        for i in range(len(B)):
            #print "add.reduce(multiply(A,h)) ", add.reduce(multiply(A,h))
            hnew[i] = h[i] + (B[i] - add.reduce(multiply(A[i,:],hnew)))/ A[i,i]
            #divide(add.reduce(subtract(B, multiply(hnew, A[i,:]))), A[i,i])
            #( B - h * A[i,:]) / A[i,i]
        dmax = max(abs(divide(subtract(h,hnew), h)))
        #print "Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
##        print_arr(h, "h")
##        print_arr(hnew, "hnew")
        if (time.time() - t) > 2.0:
            print "Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
            t = time.time()
        h[:] = hnew[:]
##        print n, ":", dmax,  h   
    print "SOLVED at: Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
    print "                           Time = ", time.time() - t1, "seconds"
    
    return h

def sor(A, B, rf, tol=0.005, init_val=0.0):
    '''itteratively solving for matrix A with solution vector B
       rf = relaxation factor (rf > 1.0; eg rf = 1.1)
       tol = tolerance for dh/h
       init_val = array of initial values to use in the solver
       
    '''
    print "successive over-relaxation (sor)"
    if init_val == 0.0:
        h = zeros(shape(B), float)
        hnew = zeros(shape(B), float)
    else:
        h = init_val
        hnew[:] = h[:]
    dmax = 1.0
    n = 0

    t1 = t = time.time()
    while dmax > tol:
        n += 1
        for i in range(len(B)):
            #print "add.reduce(multiply(A,h)) ", add.reduce(multiply(A,h))
            hnew[i] = h[i] + (B[i] - add.reduce(multiply(A[i,:],hnew)))/ A[i,i]

        #over-relaxation
        dh = subtract(hnew, h)
        hnew = h + multiply(rf,dh)
        dmax = max(abs(divide(subtract(h,hnew), h)))
        if (time.time() - t) > 2.0:
            print "Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
            t = time.time()
        
##        print_arr(h, "h")
##        print_arr(hnew, "hnew")
        h[:] = hnew[:]
    print "SOLVED at: Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
    print "                           Time = ", time.time() - t1, "seconds"
    
    return h


