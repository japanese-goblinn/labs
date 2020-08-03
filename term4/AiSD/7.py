nums = [int(i) for i in input().split()]
f = abs((nums[4] - nums[2]))
s = abs((nums[5] - nums[3]))
if f != s:
    print("YES")
else:
    print("NO")
