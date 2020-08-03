inpt = input().rstrip()
n_as_num = int(inpt)
n = list(inpt)
nums = []
lenth = len(n)
for i in range(lenth):
    find = None
    ns = sorted(n[i:])
    for j in range(len(ns)):   
        if n[i] < ns[j]:
            find = ns[j]
            break
    if find is not None:
        pos = None
        for k in range(i+1, lenth):
            if n[k] == find:
                pos = k
                break
        if pos is not None:        
            ncp = n[:]
            ncp[i], ncp[pos] = ncp[pos], ncp[i]
            pre_rs = ncp[:i+1] + sorted(ncp[i+1:])
            nums.append(int(''.join(pre_rs)))
try:
    res = min(list(filter(lambda x: x > n_as_num, list(map(int, nums)))))
    print(res)
except ValueError:
