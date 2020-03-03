def mul(m, v):
    n = len(v)
    res = []
    for i in range(n):
        row_res = 0
        for j in range(n):
            row_res += m[i][j] * v[j]
        res.append(row_res)
    return res


def main():
    n, i = map(int, input().rstrip().split())   
    a = [[float(num) for num in input().rstrip().split()] for _ in range(n)]
    b = [[float(num) for num in input().rstrip().split()] for _ in range(n)]
    x = [float(num) for num in input().rstrip().split()]

    l = mul(b, x)
    l_i = l[i - 1]

    if l_i == 0:
        print("NO")
        return
    else:
        print("YES")
    cof = -1 / l_i

    l[i - 1] = -1
    l_r = l

    for k in range(n):
        l_r[k] = cof * l_r[k]
    
    for k in range(n):
        b_r = ""
        for j in range(n):
            if k == i - 1:
                b_r += "{} ".format(l_r[k] * b[k][j])
            else:
                b_r += "{} ".format(b[k][j] + l_r[k] * b[i - 1][j])
        print(b_r)

if __name__ == "__main__":
    main()
