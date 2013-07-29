import sys
import os
import numpycbe as np


min = 0
max = 1
div = 1

if len( sys.argv ) > 1:
    max = float(sys.argv[1])
if len( sys.argv ) > 2:
    min = float(sys.argv[2])
if len( sys.argv ) > 3:
    div = float(sys.argv[3])

np.SetBlockSize(4)

size = 8

a=np.create(size,size,1)
b=np.create(size,size,1)

print "MIN="+str(min)+", MAX="+str(max)

np.random(a)
#  (HighValue - LowValue + 1) * Rnd + LowValue
for i in range(size):
    for j in range(size):
        x = np.index(a,i,j)
        x = round(((max-min+1)*x+min)/div)
        np.setIndex(a,x,i,j)

np.Print(a)

print "C CODE:"

for i in range(size):
    for j in range(size):
#        print "A[" + str(i*size*2+j) + "]="+str(np.index(a,i,j)) + ";",
        print "A[" + str(j*size+i) + "]=%.30e;" % np.index(a,i,j)

    print ""


np.Strsm_lunn(a,b)

np.PrintS(b)

