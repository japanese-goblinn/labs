n = input()
mas = [int(i) for i in input().split(' ')]
_min = mas[0]
max1 = mas[0]
max2 = 0
try:
	if len(mas) == 1:
		raise ValueError
	depth = 0
	i = 0
	while True:
		while mas[i] >= mas[i+1]:
			_min = mas[i+1]
			i += 1
		while mas[i] <= mas[i+1]:
			max2 = mas[i+1]
			i += 1
		depth += max2 - _min
		if max2 >= max1:
			max1 = max2
			_min = max1
		else:
			i -= 1
			mas[i] = max1
			_min = max1
		max2 = 0
except IndexError:
	depth += max(max1, max2) - _min
	print(depth)
except ValueError:
