import numpycbe as np

np.SetBlockSize(4)

size = 8

a=np.create(size,size,1)
b=np.create(size,size,2)
c=np.create(size,size,3)

np.Print(a)
np.Print(b)
np.Print(c)

np.Sgemm(a,b,c)

np.Print(c)

