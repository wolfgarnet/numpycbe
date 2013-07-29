from numpy import *
from matrix_solvers import *

def print_arr(arr, title):
    print title, shape(arr)
    print arr

A = array( (4, -1, -1, 0, -1, 4, 0, -1, -1, 0, 4, -1, 0, -1, -1, 4),float)
A = reshape(A, (4,4))
print_arr( A, "A")

B = array( (1,2,0,1), float)
print_arr( B, "B")

print "---"

C = jacobi(A,B)
print_arr(C, "C jacobi")

C = gauss_seidel(A,B)
print_arr(C, "C gauss_seidel")

C = sor(A, B, rf=1.3)
print_arr(C, "C sor")
