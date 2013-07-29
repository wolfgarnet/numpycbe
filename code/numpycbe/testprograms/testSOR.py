#import numpycbe

#A=numpycbe._createSOR(2,2)
#C=numpycbe._createSOR(1,1) #This is an implicit result vector
#B=numpycbe._createVector(1,1)
#print 'A just after creation'
#numpycbe._readData(A)
#print 'B just after creation'
#numpycbe._readData(B)

#h=numpycbe.create(1,1,0)
#hnew=numpycbe.create(1,1,0)

#for i in range(8):#this should be len(B) instead, but len(B) must be equal to the number of elements in a row in A to begin with!
#    print 'inside loop begin'
#    numpycbe.SetIndex(hnew,i, numpycbe.Index(h,i)+ (numpycbe.Index(B,i) - numpycbe.sum(numpycbe._sliceMult(numpycbe._sliceRow(A,i),hnew,C)))/numpycbe.Index(A,i,i))
#    print 'inside loop end'

#numpycbe.Delete(C)

import time
import numpycbe

A=numpycbe._createSOR(2,2)
#C=numpycbe._createVector(1,1) #This is an implicit result vector
C=numpycbe.create(1,8,0)
B=numpycbe._createVector(2,2)
#print 'A just after creation'
#numpycbe._readData(A)
#print 'B just after creation'
#numpycbe._readData(B)

h=numpycbe.create(1,8,0)
hnew=numpycbe.create(1,8,0)
dh=numpycbe.create(1,8,0)
dh_mult=numpycbe.create(1,8,0)
sub_temp=numpycbe.create(1,8,0)
#print 'hnew before loop is:'
#numpycbe._readData(hnew)

tol=0.005
rf=numpycbe.create(1,1,1.3)

print "successive over-relaxation (sor)"
dmax = 1.0
n = 0
t1 = t = time.time()
while dmax > tol:
	n += 1
	for i in range(8):#this should be len(B) instead, but len(B) must be equal to the number of elements in a row in A to begin with!
    		#print 'inside loop begin'
    		h_index = numpycbe.Index(h,i)
    		#print '...h_index is: ' + str(h_index)
    		B_index = numpycbe.Index(B,i)
    		#print '...B_index is: ' + str(B_index)
    		the_sum = numpycbe.sum(numpycbe._sliceMult(numpycbe._sliceRow(A,i),hnew,C))
    		#print '...the_sum is: ' + str(the_sum)
    		A_index = numpycbe.Index(A,i,i)
    		#print '...A_index is: ' + str(A_index)
    		K = h_index + (B_index - the_sum)/A_index
    		#print '...K is: ' + str(K)
    		#print '......i is' + str(i)
    		numpycbe.SetIndex(hnew,K,i)
    		#print 'inside loop end'
	#over-relaxation
	#print 'loop is done'
	#print 'hnew is:'
	#numpycbe._readData(hnew)
	#print 'h is:'
	#numpycbe._readData(h)
        numpycbe.subtractSOR(hnew,h,dh)
	#print 'dh is:'
	#numpycbe._readData(dh)
	#print 'rf is:'
	#numpycbe._readData(rf)
	numpycbe.multiplySOR(rf,dh,dh_mult)
	#print 'dh_mult is:'
	#numpycbe._readData(dh_mult)
        numpycbe.addSOR(h,dh_mult,hnew)
	#print 'hnew after add is:'
	#numpycbe._readData(hnew)
	numpycbe.subtractSOR(h,hnew,sub_temp)
	#print 'sub_temp is:'
	#numpycbe._readData(sub_temp)
	dmax = numpycbe.max(numpycbe.abs(numpycbe.divide(sub_temp, h)))
	if (n==1):
		dmax=1
	#print 'dmax is:' + str(dmax)
        if (time.time() - t) > 2.0:
            print "Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
            t = time.time()
        numpycbe._equalSOR(h,hnew)  #Okay, we need some deallocation here
	#print 'h is set equal to hnew'
	#print 'h is:'
	#numpycbe._readData(h)
	#print 'hnew is:'
	#numpycbe._readData(hnew)

print "SOLVED at: Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
print "                           Time = ", time.time() - t1, "seconds"

#numpycbe.Delete(C)
print 'hnew after loop is:'
numpycbe._readData(hnew)






















#This is the original testprogram
#def sor(A, B, rf, tol=0.005, init_val=0.0):
#    '''itteratively solving for matrix A with solution vector B
#       rf = relaxation factor (rf > 1.0; eg rf = 1.1)
#       tol = tolerance for dh/h
#       init_val = array of initial values to use in the solver
       
#    '''
#    print "successive over-relaxation (sor)"
#    if init_val == 0.0:
#        h = zeros(shape(B), float)
#        hnew = zeros(shape(B), float)
#    else:
#        h = init_val
#        hnew[:] = h[:]
#    dmax = 1.0
#    n = 0

#    t1 = t = time.time()
#    while dmax > tol:
#        n += 1
#        for i in range(len(B)):
            #print "add.reduce(multiply(A,h)) ", add.reduce(multiply(A,h))
#            hnew[i] = h[i] + (B[i] - add.reduce(multiply(A[i,:],hnew)))/ A[i,i]

        #over-relaxation
#        dh = subtract(hnew, h)
#        hnew = h + multiply(rf,dh)
#        dmax = max(abs(divide(subtract(h,hnew), h)))
#        if (time.time() - t) > 2.0:
#            print "Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
#            t = time.time()
 
##        print_arr(h, "h")
##        print_arr(hnew, "hnew")
#        h[:] = hnew[:]
#    print "SOLVED at: Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
#    print "                           Time = ", time.time() - t1, "seconds"
    
#    return h
