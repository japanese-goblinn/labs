//
//  main.cpp
//  max_dist
//
//  Created by Kirill on 12/16/20.
//

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

#define print_with_endl(x) std::cout << x << "\n"

typedef std::pair<long long, long long> node_and_cost;
typedef std::vector<std::vector<node_and_cost>> graph;

void MaxCost(graph& g,
             vector<bool>& visited,
             vector<long long>& costs,
             vector<bool>& isCostUpdated,
             long long node)
{
    visited.at(node) = true;
    for (size_t i = 0; i < g.at(node).size(); ++i) {
        auto [nextNode, weight] = g.at(node).at(i);
        if (!visited.at(nextNode))
            MaxCost(g, visited, costs, isCostUpdated, nextNode);
        if (!isCostUpdated.at(nextNode))
            continue;
        costs.at(node) = max(costs.at(nextNode) + weight, costs.at(node));
        isCostUpdated.at(node) = true;
    }
}

int main(int argc, const char * argv[]) {
    size_t n, m;
    cin >> n >> m;
    
    graph graph(n);
    long long a, b, w;
    for (size_t i = 0; i < m; ++i) {
        cin >> a >> b >> w;
        graph.at(a - 1).push_back({ b - 1, w });
    }
    
    auto visited = vector<bool>(n);
    auto costs = vector<long long>(n);
    auto isCostUpdated = vector<bool>(n);
    
    isCostUpdated.back() = true;
    long long startNode = 0;
    
    MaxCost(graph, visited, costs, isCostUpdated, startNode);
    print_with_endl(costs.front());
    
    return 0;
}

