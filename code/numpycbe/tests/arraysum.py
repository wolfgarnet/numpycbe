import numpycbe as np

np.SetBlockSize(4)

size = 8

a=np.create(size,size,1)

np.Print(a)

print "Summing a to x"

x = np.sum( a )

print "x is " + str(x)

b=np.arange(0,10,1)

np.Print(b)

print "Summing b to y"

y = np.sum( b )

print "y is " + str(y)
