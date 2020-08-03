import numpy as np
from math import sqrt, log, ceil


def cof_input(i, state):
    while True:
        try:
            show = 'x' if state == 0 else 'b'
            var = float(input(f"Коэффициент при {show}{i+1}: "))
        except ValueError:
            print("Неверное значение, попробуйте снова")
        else:
            return var


def column(A, j):
    return [row[j] for row in A]


def check_convergence(B):
    norm1 = max(sum(map(abs, B[i])) for i in range(len(B))) < 1
    sum_B_sq = 0
    for i in range(len(B)):
        sum_B_sq += sum([ij*ij for ij in B[i]])
    norm2 = sqrt(sum_B_sq) < 1
    norm3 = max(sum(map(abs, j)) for j in zip(*B)) < 1
    eig_abs_vals = map(abs, np.linalg.eigvals(B))
    eigv = all(x < 1 for x in eig_abs_vals)
    if norm1 or norm2 or norm3 or eigv:
        return True
    return False


def diagonal_priority(A):
    if np.allclose(np.array(A), np.array(A).transpose(), 1e-8) and \
            np.all(np.linalg.eigvals(A) > 0):
        return True
    for i in range(len(A)):
        if not (2 * abs(A[i][i]) - sum(map(abs, A[i])) > 0 or 2 * abs(A[i][i])
                - sum(map(abs, column(A, i))) > 0): 
            return False
    return True


n = int(input("Введите размерность матрицы: "))

print("\nВведите коэффициенты при \'x\'", end='\n')
a = np.array([[cof_input(i, 0) for i in range(n)] for j in range(n)])

print("\nВведите свободные коэффициенты", end='\n\n')
b = np.array([cof_input(i, 1) for i in range(n)])

print("\nМатрица A:\n")
print(a, end='\n\n')
print("Вектор свободных членов b:\n")
print(b, end='\n\n')

# метод простых итераций
try:
    for i in range(len(a)):
        d = a[i][i]
        for j in range(len(a)):
            a[i][j] = a[i][j] / d
            
        b[i] = b[i] / a[i][i]
    print("\nМатрица A:\n")
    print(a, end='\n\n')    
    x0 = b[:]
    k0 = 0
    
    E = np.diag([1] * len(a))
    B = [[1.0 for i in range(len(a))] for j in range(len(a))]
    for i in range(len(a)):
        B[i] = [e - a for (e, a) in zip(E[i], a[i])]
   
    if check_convergence(B):
        eps = 0.01
        x = [m + n for (m, n) in zip(np.dot(B, x0), b)]
        k0 = ceil(log((eps*(1 - np.linalg.norm(B)) /
                       np.linalg.norm(np.subtract(x, x0)))) /
                  log(np.linalg.norm(B)))
        x0 = x
        for _ in range(k0 - 1):
            x = [m + n for (m, n) in zip(np.dot(B, x0), b)]
            print(x)
            x0 = x
        x, k = x0, k0
    else:
        raise ValueError

    format_roots = [f"x{i + 1}: {x[i]:.2f}; " for i in range(len(x))]
    print("Корни СЛАУ методом простых итераций: ", "".join(format_roots))
    print('\nКоличесвто итераций k0 = ' + str(k))
except ValueError:
    print("Нет сходимости")

# метод Зейделя
if diagonal_priority(a[:]):
    n = len(a)
    x = [0.0 for i in range(n)]
    while True:
        xn = np.copy(x)
        for i in range(n):
            s1 = sum(a[i][j] * xn[j] for j in range(i))
            s2 = sum(a[i][j] * x[j] for j in range(i + 1, n))
            xn[i] = (b[i] - s1 - s2) / a[i][i]
        if np.linalg.norm(xn - x) < 0.01:
            break
        x = xn
    format_roots = [f"x{i + 1}: {x[i]:.2f}; " for i in range(len(x))]
    print("\nКорни СЛАУ методом Зейделя: ", "".join(format_roots))
else:
    print("Нет диагонального приобладания")
