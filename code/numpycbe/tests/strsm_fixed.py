import numpycbe as np

np.SetBlockSize(4)

size = 4

a=np.create(size,2*size,1)


# 	A[0]  = 64; A[1]    = 65;A[2]         = 66;A[3]                = 67;
#	A[8]  = 0.125; A[9] = 0.875;A[10]     = 1.75;A[11]             = 2.625;
#	A[16] = 0.25;A[17]  = 0.857142925262451171875;A[18] = -0.00000011920928955078125;A[19] = -0.000000178813934326171875;
#	A[24] = 0.625;A[25] = 0.4285714626312255859375;A[26] = 0.5;A[27]              = 0;
#


# A:
#        o  v x y
np.SetIndex(a,64,0,0)
np.SetIndex(a,65,1,0)
np.SetIndex(a,66,2,0)
np.SetIndex(a,67,3,0)

#        o  v x y
np.SetIndex(a,0.125,0,1)
np.SetIndex(a,0.875,1,1)
np.SetIndex(a,1.75,2,1)
np.SetIndex(a,2.625,3,1)

#        o  v x y
np.SetIndex(a,0.25,0,2)
np.SetIndex(a,0.85714292526245117187,1,2)
np.SetIndex(a,-0.00000011920928955078125,2,2)
np.SetIndex(a,-0.00000017881393432617187,3,2)

#        o  v x y
np.SetIndex(a,0.625,0,3)
np.SetIndex(a,0.4285714626312255859375,1,3)
np.SetIndex(a,0.5,2,3)
np.SetIndex(a,0,3,3)


#	A[4] = 68;A[5] = 69;A[6] = 70;A[7] = 71;
#	A[12] = 12;A[13] = 13;A[14] = 14;A[15] = 15;
#	A[20] = 20;A[21] = 21;A[22] = 22;A[23] = 23;
#	A[28] = 44;A[29] = 45;A[30] = 46;A[31] = 47;

# B:
#        o  v x y
np.SetIndex(a,68,4,0)
np.SetIndex(a,69,5,0)
np.SetIndex(a,70,6,0)
np.SetIndex(a,71,7,0)

#        o  v x y
np.SetIndex(a,12,4,1)
np.SetIndex(a,13,5,1)
np.SetIndex(a,14,6,1)
np.SetIndex(a,15,7,1)

#        o  v x y
np.SetIndex(a,20,4,2)
np.SetIndex(a,21,5,2)
np.SetIndex(a,22,6,2)
np.SetIndex(a,23,7,2)

#        o  v x y
np.SetIndex(a,44,4,3)
np.SetIndex(a,45,5,3)
np.SetIndex(a,46,6,3)
np.SetIndex(a,47,7,3)


np.PrintS(a)


np.Strsm_lu(a,a,1,0)

np.PrintS(a)

