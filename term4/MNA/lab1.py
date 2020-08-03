import numpy as np
import sys


def cof_input(i, state):
    while True:
        try:
            show = 'x' if state == 0 else 'b'
            var = float(input(f"Коэффициент при {show}{i+1}: "))
        except ValueError:
            print("Неверное значение, попробуйте снова")
        else:
            return var


n = int(input("Введите размерность матрицы: "))

print("\nВведите коэффициенты при \'x\'", end='\n')
x = [[cof_input(i, 0) for i in range(n)] for j in range(n)]
x_copy = [row[:] for row in x]

print("\nВведите свободные коэффициенты", end='\n')
b = [cof_input(i, 1) for i in range(n)]

norm_b = max(b)

print("\nМатрица:", end='\n')
for s in x:
    print(s, end='\n')

print("\nСвободные коэффициенты:", end='\n')
for s in b:
    print(s, end='\n')

count = 0
for j in range(count, n - 1):
    for i in range(count, n - 1):
        if not x[j][j]:
            x[j], x[j + 1] = x[j + 1], x[j]
            b[j], b[j + 1] = b[j + 1], b[j]
        m = - x[i + 1][j] / x[j][j]
        mx = [m*l for l in x[j]]
        b[i + 1] += m * b[j]
        for k in range(n):
            x[i + 1][k] += mx[k]
    count += 1

for i in range(n):
    x[i].reverse()
x.reverse()
b.reverse()

roots_vect = []
for i in range(n):
    for j in range(n):
        if i == n - 1:
            for k in range(n - 1):
                b[i] -= roots_vect[k] * x[i][k]
            roots_vect.append(b[i] / x[i][n - 1])
            break
        if x[i][j + 1] == 0:
            roots_vect.append(b[i] / x[i][j])
            break
        else:
            b[i] -= roots_vect[j] * x[i][j]

roots_vect.reverse()
format_roots = [f"x{i + 1}: {roots_vect[i]}; " for i in range(len(roots_vect))]
print("\nКорни СЛАУ: ", "".join(format_roots))

print("\nОбратная матрица системы:", end='\n')
try:
    inv = np.linalg.inv(x_copy)
except np.linalg.LinAlgError:
    print("Невозможно найти обратную матрицу")
else:
    print("{}".format(inv))
    norm_x = max(sum(x_copy[i]) for i in range(n))
    norm_inv = max(sum(inv[i]) for i in range(n))
    norm_roots = max(roots_vect)
    abs_b = 0.001
    rel_b = abs_b / norm_b
    abs_x = norm_inv * abs_b
    rel_x = norm_x * norm_inv * rel_b
    print(f"\nАбсолютная погрешность x = {abs_x}",
          f"Относительная погрешность x <= {rel_x}\n")
