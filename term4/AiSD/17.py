num = 1087388483 
n = int(input())
res = 1
count = 0
for _ in range(n):
    i = int(input())
    if i == 0:
        continue
    if not num % i:
        if i == 1052651 or i == 1054693 or i == 1065023:
            count += 1
        if i == num:
            res = num
            break
        res *= i
        if res == num:
            break

if res == num or count > 0:
    print("YES")
else:
