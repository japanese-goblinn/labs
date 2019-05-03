n = int(input())
nums = [int(i) for i in input().split(' ')]
if n == 2:
    print(nums[0] * nums[1])
else:
    nums.sort()
    res1 = nums[0] * nums[1]
    res2 = nums[-1] * nums[-2]
    if res1 < res2:
        print(res2)
    else:
        print(res1)
