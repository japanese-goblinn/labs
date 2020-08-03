import math
import numpy as np

n = int(input("Введите размерность матрицы: "))

print("\nВведите матрицу A", end='\n\n')
a_input = np.array([[float(i) for i in input().split()] for j in range(n)])

print("\nВведите свободные коэффициенты b", end='\n\n')
b_input = np.array([float(i) for i in input().split()])

u = np.array([[0.0 for i in range(n)] for j in range(n)])

#симметризация системы 
a_transporent = np.transpose(a_input)
a = a_transporent @ a_input
b = a_transporent @ b_input
print("\nСимметризированная система:\n")
print(a, end='\n\n')
print(b, end='\n\n')

#решение методом квадратного корня
count = 0
for i in range(count, n):
    for j in range(count, n):
        if not i: 
            try:
                u[i][i] = math.sqrt(a[i][i])
            except ValueError:
                raise Exception("Матрица не является положительно опредленной")
            else:    
                for k in range(1, n):
                    u[i][k] = a[i][k] / u[i][i]
                break
        else:
            if i == j:
                try:
                    u[i][i] = math.sqrt(a[i][i] - sum([u[k][i]**2 for k in range(i)]))
                except ValueError:
                    raise Exception("Матрица не является положительно опредленной")
            else:
                u[i][j] = (a[i][j] - sum([u[k][i]*u[k][j] for k in range(i)])) / u[i][i]
    count += 1


u_transporent = np.transpose(u)
u_mul_y = np.linalg.inv(u_transporent)
y = u_mul_y @ b
x = u_transporent @ y
format_roots = [f"x{i + 1}: {x[i]}; " for i in range(len(x))]
print("Корни СЛАУ: ", "".join(format_roots))

#определитель матрицы А
det_a = 1
for i in (u[i][i]**2 for i in range(n)):
    det_a *= i
print(f"\nОпределитель матрицы A:\n\n{det_a}", end='\n\n')

#обратная матрица 
inv_a = np.array([[0.0 for i in range(n)] for j in range(n)])
for i in range(n):
    e_i = np.array([[0.0]] * n)
    e_i[i] = 1.0
    y = np.linalg.inv(u_transporent) @ e_i
    x_new = np.linalg.inv(u) @ y
    for j in range(n):
        inv_a[j][i] = x_new[j]
print(f"Обратная матрица:\n\n{inv_a}", end='\n\n\n')

print(np.linalg.inv(a))