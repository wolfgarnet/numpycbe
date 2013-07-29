import numpycbe as np

np.SetBlockSize(4)

size = 8

a=np.create(size,size,1)
b=np.create(size,size,1)

np.Print(a)

for i in range(size):
    for j in range(size):
        np.SetIndex(a,size*size-i*size+j,j,i)
#        np.SetIndex(a,i+(j/10.0),j,i)

#        a[j,i] = size*size-i*size+j

np.Print(a)

np.solve(a,b)


print "--------------------------"
print "--------------------------"

np.Print(a)
np.Print(b)
