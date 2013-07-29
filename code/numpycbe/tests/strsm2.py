import numpycbe as np

np.SetBlockSize(4)

size = 4

a=np.create(size,2*size,1)

for i in range(size):
    for j in range(size*2):
        np.SetIndex(a,i*size*2+j,j,i)

np.Print(a)


np.Strsm_lu(a,a,1,0)

np.Print(a)

