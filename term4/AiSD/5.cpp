#include <iostream>
#include <vector>
#include <set>
#include <climits>

long long inf = LLONG_MAX;

int main(int argc, const char * argv[]) {
    long long n, m, a, b, c;
    std::cin >> n >> m;
    std::set<std::pair<long long, long long>> set;
    std::vector<std::vector<std::pair<long long, long long>>> graph(n);
    std::vector<long long> costs(n);
    for (long long i = 0; i < m; i++) {
        std::cin >> a >> b >> c;
        graph[a - 1].push_back(std::make_pair(c, b - 1));
        costs[a - 1] = inf;
        graph[b - 1].push_back(std::make_pair(c, a - 1));
        costs[b - 1] = inf;
    }
    long long A, B;
    std::cin >> A >> B;
    costs[A - 1] = 0;
    set.insert(std::make_pair(0, A - 1));
    while (!set.empty()) {
        long long current_node_cost, node;
        std::set<std::pair<long long, long long>>::iterator set_begin = set.begin();
        current_node_cost = set_begin->first;
        node = set_begin->second;
        set.erase(set_begin);
        for (long long i = 0; i < graph[node].size(); i++) {
            long long to_node_cost = graph[node][i].first;
            long long to_node = graph[node][i].second;
            if (costs[to_node] > costs[node] + to_node_cost) {
                set.erase(std::make_pair(costs[to_node], to_node));
                costs[to_node] = costs[node] + to_node_cost;
                set.insert(std::make_pair(costs[to_node], to_node));
            }
        }
    }
    std::cout << costs[B - 1];
