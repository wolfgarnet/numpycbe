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

print "Scaling column 0 with 2.5"

np.ScaleCol(a, 2.5,0,0)

np.Print(a)

print "Scaling column 1 from row 2 with 2"

np.ScaleCol(a, 2,1,2)

np.Print(a)
