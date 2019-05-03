n, m, i, j = map(int, input().split())
mov_x = [2, 2, -2, -2, 1, 1, -1, -1]
mov_y = [1, -1, 1, -1, 2, -2, 2, -2]
queue = []
visited = {}
result = 0

def is_valid(x, y):
    return x <= n and x > 0 and y <= m and y > 0

if i != 1 or j != 1:
    start = (1, 1, 0)
    queue.append(start)
    while queue:
        node = queue.pop(0)
        if node[0] == i and node[1] == j:
            result = node[2]
            break
        if not visited.get((node[0], node[1]), False):
            visited[node[0], node[1]] = True
            for k, l in zip(mov_x, mov_y):
                if is_valid(node[0] + k, node[1] + l):
                    queue.append((node[0] + k, node[1] + l, node[2] + 1))
    print(result if result else 'NEVAR')
else:
