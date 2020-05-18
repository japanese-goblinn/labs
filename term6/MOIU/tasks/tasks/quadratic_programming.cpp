//
//  quadratic_programming.cpp
//  tasks
//
//  Created by Kirill Gorbachyonok on 5/17/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>
#include <cmath>
#include <numeric>

#define EPSILON 1e-5
#define endlPrint(x) std::cout << x << "\n"
#define answerPrint(x) std::cout.precision(15); std::cout << std::fixed << x << " ";

typedef std::vector<int> indexes;
typedef std::vector<long double> vector;
typedef std::vector<std::vector<long double>> matrix;

void outputFor(matrix &m) {
    for (unsigned long i = 0; i < m.size(); ++i) {
        for (unsigned long j = 0; j < m.at(0).size(); ++j) {
            answerPrint(m.at(i).at(j));
        }
        std::cout << "\n";
    }
}

void outputFor(vector &v) {
    for (auto const &e : v) {
        answerPrint(e);
    }
    std::cout << "\n";
}

void inputFor(matrix &matrix) {
    for (unsigned long i = 0; i < matrix.size(); ++i)
        for (unsigned long j = 0; j < matrix.at(0).size(); ++j)
            std::cin >> matrix.at(i).at(j);
}

template <typename T>
void inputFor(std::vector<T> &vector) {
    for (unsigned long i = 0; i < vector.size(); ++i)
        std::cin >> vector.at(i);
}

void mulInPlace(vector &v, double value) {
    for (unsigned long i = 0; i < v.size(); ++i)
        v.at(i) *= value;
}

vector mul(vector const &v, double value) {
    vector nv(v.size());
    for (unsigned long i = 0; i < v.size(); ++i)
        nv.at(i) = value * v.at(i);
    return nv;
}

void addInPlace(vector &row, vector const &added_row) {
    for (unsigned long j = 0; j < row.size(); ++j)
        row.at(j) += added_row.at(j);
}

matrix inverseMatrixOf(matrix const &m) {
    auto size = m.size();
    matrix aug_matrix = m;
    vector v(size);
    for (unsigned long i = 0; i < size; ++i) {
        v.at(i) = 1;
        aug_matrix.at(i).insert(std::end(aug_matrix.at(i)), std::begin(v), std::end(v));
        v.at(i) = 0;
    }
    for (unsigned long i = 0; i < size; ++i) {
        auto d_el = aug_matrix.at(i).at(i);
        if (d_el == 0) {
            for (auto j = i + 1; j < size; ++j) {
                if (aug_matrix.at(j).at(i) != 0) {
                    aug_matrix.at(j).swap(aug_matrix.at(i));
                    d_el = aug_matrix.at(i).at(i);
                    break;
                }
            }
        }
        for (unsigned long j = 0; j < size; ++j) {
            if (j == i)
                continue;
            auto x = -1 * aug_matrix.at(j).at(i) / d_el;
            auto new_row = mul(aug_matrix.at(i), x);
            addInPlace(aug_matrix.at(j), new_row);
        }
        mulInPlace(aug_matrix.at(i), 1 / d_el);
    }
    matrix inversed(size);
    for (unsigned long i = 0; i < size; ++i)
        inversed.at(i) = vector(aug_matrix.at(i).begin() + size, aug_matrix.at(i).end());
    return inversed;
}

void quadraticProgramming() {
    
}

int main(int argc, const char * argv[]) {
    int m, n, k;
    std::cin >> m >> n >> k;
    matrix a(m, vector(n));
    vector b(m);
    vector c(n);
    matrix d(n, vector(n));
    vector x(n);
    indexes j_op(m);
    indexes j_extd(k);
    inputFor(a);
    inputFor(b);
    inputFor(c);
    inputFor(d);
    inputFor(x);
    inputFor(j_op);
    inputFor(j_extd);
    for (auto &j : j_op)
        j--;
    for (auto &j : j_extd)
        j--;
    quadraticProgramming();
    return 0;
}
