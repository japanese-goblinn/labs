N, N_TIMES = tuple(map(int, input().split()))
nums = list(map(int, input().split()))
mod = 10 ** 9 + 7
nums.sort()
res = 1
if nums[-1] < 0:
    if N_TIMES % 2:
        for i in range(N_TIMES):
            res *= nums[-1 - i]
            res %= mod
    else:
        for i in range(N_TIMES):
            res *= nums[i]
            res %= mod
else:
    if N_TIMES % 2:
        res *= nums.pop(-1)
        res %= mod
        N_TIMES -= 1
    i = 0
    j = -1
    for _ in range(N_TIMES // 2):
        res1 = nums[i] * nums[i + 1]
        res2 = nums[j] * nums[j - 1]
        res1_is_greater = res1 > res2
        res *= res1 if res1_is_greater else res2
        res %= mod
        if res1_is_greater:
            i += 2
        else:
            j -= 2
