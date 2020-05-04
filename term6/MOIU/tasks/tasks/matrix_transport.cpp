//
//  matrix_transport.cpp
//  tasks
//
//  Created by Kirill Gorbachyonok on 4/21/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>
#include <cmath>
#include <numeric>

typedef std::pair<int, int> cell;
typedef std::vector<int> vector;
typedef std::vector<std::vector<int>> matrix;

#define EPSILON 1e-10
#define endlPrint(x) std::cout << x << "\n"
#define answerPrint(x) std::cout << x << " ";

void outputFor(matrix &m) {
    for (unsigned long i = 0; i < m.size(); ++i) {
        for (unsigned long j = 0; j < m.at(0).size(); ++j) {
            answerPrint(m.at(i).at(j));
        }
        std::cout << "\n";
    }
}

void outputFor(vector &v) {
    for (auto const &e: v) {
        answerPrint(e);
    }
    std::cout << "\n";
}

void inputFor(matrix &matrix) {
    for (unsigned long i = 0; i < matrix.size(); ++i)
        for (unsigned long j = 0; j < matrix.at(0).size(); ++j)
            std::cin >> matrix.at(i).at(j);
}

void inputFor(vector &vector) {
    for (unsigned long i = 0; i < vector.size(); ++i)
        std::cin >> vector.at(i);
}

void addColumn(matrix &m, int value = 0) {
    for (auto &row: m)
        row.emplace_back(value);
}

void addRow(matrix &m, int value = 0) {
    m.emplace_back(vector(m.at(0).size(), value));
}

void balance(matrix &x, matrix &c, vector &a, vector &b) {
    auto stocks_sum = std::accumulate(a.begin(), a.end(), 0);
    auto stores_sum = std::accumulate(b.begin(), b.end(), 0);
    if (stocks_sum < stores_sum) {
        a.emplace_back(stores_sum - stocks_sum);
        addRow(c);
        addRow(x);
    }
    if (stocks_sum > stores_sum) {
        b.emplace_back(stocks_sum - stores_sum);
        addColumn(c);
        addColumn(x);
    }
}

void matrixTransportTask(matrix &x, matrix &c, vector &a, vector &b) {
    balance(x, c, a, b);
    auto used = a;
    auto need = b;
    unsigned long i = 0;
    unsigned long j = 0;
    std::vector<cell> basis;
    while (j < x.at(0).size()) {
        x.at(i).at(j) = std::min(used.at(i), need.at(j));
        auto diff = used.at(i) - need.at(j);
        basis.emplace_back(cell(i, j));
        if (diff > 0) {
            used.at(i) -= x.at(i).at(j);
            j++;
        } else if (diff < 0) {
            need.at(j) -= x.at(i).at(j);
            i++;
        } else {
            if (j + 1 != x.at(0).size())
                basis.emplace_back(cell(i, j + 1));
            j++;
            i++;
        }
    }
    while (true) {
        for (auto &b : basis) {
            
        }
    }
}

int main(int argc, const char * argv[]) {
    int m, n;
    std::cin >> m >> n;
    matrix c(m, vector(n));
    matrix x(m, vector(n));
    vector a(m);
    vector b(n);
    inputFor(c);
    inputFor(a);
    inputFor(b);
    matrixTransportTask(x, c, a, b);
    return 0;
}
