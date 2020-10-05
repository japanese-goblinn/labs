//
//  main.cpp
//  saio
//
//  Created by Kirill Gorbachyonok on 10/4/20.
//

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

#define print_with_endl(x) std::cout << x << "\n"

typedef std::pair<long long, long long> cost_and_node;
typedef std::pair<long long, long long> node_and_cost;
typedef std::vector<std::vector<node_and_cost>> graph;

auto Dijkstra(graph& g) {
    auto start = 0;
    auto destination = (long long)(g.size() - 1);
    vector<long long> d(g.size(), LLONG_MAX);
    d.at(start) = 0;
    priority_queue<cost_and_node> q;
    q.push({ 0, start });
    while (!q.empty()) {
        auto cur_cost = -q.top().first;
        auto node = q.top().second;
        q.pop();
        if (cur_cost > d.at(node)) {
            continue;
        }
        for (size_t i = 0; i < g.at(node).size(); ++i) {
            auto next_node = g.at(node).at(i).first;
            auto check_cost = g.at(node).at(i).second;
            if (d.at(node) + check_cost >= d.at(next_node)) {
                continue;
            }
            d.at(next_node) = d.at(node) + check_cost;
            q.push({ -d.at(next_node), next_node });
        }
    }
    return d.at(destination);
}

int main(int argc, const char * argv[]) {
    ifstream f("input.txt");
    if (!f.is_open()) {
        return -9;
    }
    size_t n, m;
    f >> n >> m;
    graph graph(n);
    long long u, v, w;
    for (size_t i = 0; i < m; ++i) {
        f >> u >> v >> w;
        graph.at(u - 1).push_back({ v - 1, w });
        graph.at(v - 1).push_back({ u - 1, w });
    }
    auto cost = Dijkstra(graph);
    ofstream o("output.txt");
    o << cost;
    return 0;
}
