import sys
sys.path.append( '../lib/python2.4/site-packages/' )
import numpycbe as np

np.SetBlockSize(4)

size = 8

a = np.create(size,size,1)

for i in range(size):
    for j in range(size):
        np.SetIndex(a,i+1,j,i)

np.Print(a)

print "Swapping rows 1 and 2"

np.SwapRows(a,1,2)

np.Print(a)

print "Swapping rows 4 and 7"

np.SwapRows(a,4,7)

np.Print(a)
