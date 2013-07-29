import time
import numpycbe

blockSize = 64
instanceSize = 16  #16/32/48/64

numpycbe.SetBlockSize(blockSize)

A=numpycbe.create(instanceSize*blockSize,instanceSize*blockSize,1)
C=numpycbe.create(1,instanceSize*blockSize,0)
B=numpycbe.create(1,instanceSize*blockSize,1)

h=numpycbe.create(1,instanceSize*blockSize,0)
hnew=numpycbe.create(1,instanceSize*blockSize,0)
dh=numpycbe.create(1,instanceSize*blockSize,0)
dh_mult=numpycbe.create(1,instanceSize*blockSize,0)
sub_temp=numpycbe.create(1,instanceSize*blockSize,0)

tol=0.005
rf=numpycbe.create(1,1,1.3)

print "successive over-relaxation (sor)"
dmax = 1.0
n = 0
t1 = t = time.time()
#while dmax > tol:
while n < 100:
	n += 1
	for i in range(instanceSize*blockSize):#this should be len(B) instead, but len(B) must be equal to the number of elements in a row in A to begin with!
    		h_index = numpycbe.Index(h,i)
    		B_index = numpycbe.Index(B,i)
    		the_sum = numpycbe.sum(numpycbe._sliceMult(numpycbe._sliceRow(A,i),hnew,C))
    		A_index = numpycbe.Index(A,i,i)
    		K = h_index + (B_index - the_sum)/A_index
    		numpycbe.SetIndex(hnew,K,i)
	#over-relaxation
        numpycbe.subtractSOR(hnew,h,dh)
	numpycbe.multiplySOR(rf,dh,dh_mult)
        numpycbe.addSOR(h,dh_mult,hnew)
	numpycbe.subtractSOR(h,hnew,sub_temp)
	dmax = numpycbe.max(numpycbe.abs(numpycbe.divide(sub_temp, h)))
	#print 'dmax is:' + str(dmax)
        #if (time.time() - t) > 2.0:
        #    print "Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
        #    t = time.time()
        numpycbe._equalSOR(h,hnew)
	

print "SOLVED at: Itteration = ", n, ":   dmax = ", dmax, ": tol = ", tol
print "                           Time = ", time.time() - t1, "seconds"

#numpycbe.Delete(C)
print 'hnew after loop is:'
numpycbe._readData(hnew)