#include <iostream>
#include <vector>
#include <numeric>

void dfs(int v, std::vector<bool> &visited, std::vector<std::vector<int>> &g) {
    visited[v] = true;
    for (int i = 0; i < g[v].size(); i++) {
        if (!visited[g[v][i]]) {
            dfs(g[v][i], visited, g);
        }
    }
}


int main(int argc, const char * argv[]) {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> g(n);
    std::vector<bool> visited(n, false);
    int u, v;
    for (int i = 0; i < m; i++) {
        std::cin >> u >> v;
        g[u - 1].push_back(v - 1);
        g[v - 1].push_back(u - 1);
    }
    dfs(0, visited, g);
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs(i, visited, g);
            count++;
        }
    }
    std::cout << count;
