//
//  main.cpp
//  dinica
//
//  Created by Kirill on 12/17/20.
//

#include <iostream>
#include <fstream>
#include <vector>

#define print_with_endl(x) std::cout << x << "\n"

using namespace std;
 
struct Edge {
    int a, b, cap, flow;
};
 
int n, s, t;
vector<int> d, ptr, q;
vector<Edge> e;
vector<vector<int>> g;
 
void AddEdge(int a, int b, int capacity) {
    Edge e1 = { a, b, capacity, 0 };
    Edge e2 = { b, a, 0, 0 };
    g.at(a).push_back(e.size());
    e.push_back(e1);
    g.at(b).push_back (e.size());
    e.push_back(e2);
}
 
auto BFS() {
    auto qh = 0, qt = 0;
    q.at(qt++) = s;
    d.assign(n, -1);
    d.at(s) = 0;
    while (qh < qt && d.at(t) == -1) {
        auto v = q[qh++];
        for (size_t i = 0; i < g.at(v).size(); ++i) {
            auto id = g.at(v).at(i);
            auto to = e.at(id).b;
            if (d.at(to) == -1 && e.at(id).flow < e.at(id).cap) {
                q.at(qt++) = to;
                d.at(to) = d.at(v) + 1;
            }
        }
    }
    return d.at(t) != -1;
}
 
auto DFS(int v, int flow) {
    if (!flow)
        return 0;
    if (v == t)
        return flow;
    for (; ptr.at(v) < g.at(v).size(); ++ptr.at(v)) {
        auto id = g[v][ptr[v]];
        auto to = e[id].b;
        if (d[to] != d[v] + 1)
            continue;
        auto pushed = DFS(to, min(flow, e.at(id).cap - e.at(id).flow));
        if (pushed) {
            e.at(id).flow += pushed;
            e.at(id ^ 1).flow -= pushed;
            return pushed;
        }
    }
    return 0;
}
 
auto Dinic() {
    auto flow = 0;
    while(true) {
        if (!BFS())
            break;
        ptr.assign(n, 0);
        while (int pushed = DFS(s, INT_MAX))
            flow += pushed;
    }
    return flow;
}

int main(int argc, const char * argv[]) {
    ifstream f("flow.in");
    if (!f.is_open()) {
        return -9;
    }
    int m;
    f >> n >> m;
    f >> s >> t;
    s--; t--;
    g = vector<vector<int>>(n, vector<int>());
    q = vector<int>(n);
    d = vector<int>(n);
    ptr = vector<int>(n);
    for (size_t i = 0; i < m; ++i) {
        int a, b, capacity;
        f >> a >> b >> capacity;
        AddEdge(a - 1, b - 1, capacity);
    }
    auto result = Dinic();
    ofstream o("flow.out");
    o << result << endl;
    for (size_t i = 0; i < e.size(); i += 2)
        o << e.at(i).flow << endl;
    return 0;
}
