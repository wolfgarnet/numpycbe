import numpycbe as np

np.SetBlockSize(4)

size = 4

a=np.create(size,2*size,1)

np.Print(a)


np.Strsm_lu(a,a,1,0)

np.Print(a)

