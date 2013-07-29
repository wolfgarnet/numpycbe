#import sys
#sys.path.append( '../lib/python2.4/site-packages/' )
import numpycbe as np

np.SetBlockSize(4)

size = 8

a = np.create(size,size,1)

np.random(a)

np.Print(a)

print "Finding the largest index in column 0"

l = np.MaxCol(a,0,0)

print "Largest = " + str(l)

print "Finding the largest index in column 2, from row 4"

l = np.MaxCol(a,2,3)

print "Largest = " + str(l)
