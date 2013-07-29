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

size = 4

a=np.create(size,2*size,1)

print "MIN="+str(min)+", MAX="+str(max)

np.random(a)
#  (HighValue - LowValue + 1) * Rnd + LowValue
for i in range(size):
    for j in range(size*2):
        x = np.index(a,i,j)
        x = ((max-min+1)*x+min)/div
        np.setIndex(a,x,i,j)

np.Print(a)

print "C CODE:"

for i in range(size):
    for j in range(size*2):
#        print "A[" + str(i*size*2+j) + "]="+str(np.index(a,i,j)) + ";",
        print "A[" + str(i*size*2+j) + "]=%.30e;" % np.index(a,i,j)

    print ""


np.Strsm_lu(a,a,1,0)

np.PrintS(a)

