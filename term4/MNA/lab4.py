import numpy as np
from math import sin, cos, atan


def max_el_above(A):
    max_el = 0
    i0 = 0
    j0 = 0
    for i in range(len(A)):
        for j in range(i+1, len(A)):
            if abs(A[i][j]) > abs(max_el):
                max_el = A[i][j]
                i0, j0 = i, j
    return max_el, i0, j0


n = int(input("Введите размерность матрицы: "))

print("\nВведите матрицу A", end='\n\n')
a = np.array([[float(i) for i in input().split()] for j in range(n)])

#метод вращений 
A = a @ np.transpose(a)
print(A, end='\n')
U_k = np.diag([1.0] * len(A))
while True:
    U = np.diag([1.0] * len(A))
    max_el, i, j = max_el_above(A)
    if abs(max_el) < 1e-10:
        break

    angle = 1/2 * atan(2 * A[i][j] / (A[i][i] - A[j][j]))
    U[i][i] = cos(angle)
    U[j][j] = cos(angle)
    U[i][j] = -sin(angle)
    U[j][i] = sin(angle)

    A = np.transpose(U) @ A @ U
    print(A)
    U_k = U_k @ U

values, vectors = np.diag(A), U_k

print(f"\nСобственные значения:\n {values}")
print(f"\nСобственные векторы:\n {vectors}")
