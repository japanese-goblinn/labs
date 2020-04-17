//
//  simplex_main.cpp
//  simplex_main
//
//  Created by Kirill Gorbachyonok on 4/1/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>
#include <cmath>

typedef std::vector<double> vector;
typedef std::vector<std::vector<double>> matrix;

#define endlPrint(x) std::cout << x << "\n"
#define answerPrint(x) std::cout.precision(15); std::cout << std::fixed << x << " ";

void outputFor(matrix &m) {
    auto n = m.at(0).size();
    for (unsigned long i = 0; i < m.size(); ++i) {
        for (unsigned long j = 0; j < n; ++j) {
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
    auto n = matrix.at(0).size();
    for (auto &row: matrix)
        for (int j = 0; j < n; ++j)
            std::cin >> row.at(j);
}

void inputFor(vector &vector) {
    for (auto &item: vector)
        std::cin >> item;
}

vector columnFrom(matrix const &m, int index) {
    vector column;
    for (auto &row: m)
        column.emplace_back(row.at(index));
    return column;
}

matrix basisMatrixFor(matrix const &m, vector const &basis_indexes) {
    auto size = basis_indexes.size();
    matrix a_b(size, vector(size));
    for (auto i = 0; i < size; ++i) {
        auto colm = columnFrom(m, basis_indexes.at(i));
        for (auto j = 0; j < size; ++j)
            a_b.at(j).at(i) = colm.at(j);
    }
    return a_b;
}

void mulInPlace(vector &v, double value) {
    for (auto &item: v)
        item *= value;
}

void addInPlace(vector &row, vector const &added_row) {
    for (auto j = 0; j < row.size(); ++j)
        row.at(j) += added_row.at(j);
}

vector mul(vector const &v, double value) {
    vector nv(v.size());
    for (auto i = 0; i < v.size(); ++i)
        nv.at(i) = value * v.at(i);
    return nv;
}

vector mul(matrix const &m, vector const &v) {
    vector result;
    for (auto i = 0; i < m.size(); ++i) {
        double row_result = 0;
        for (auto j = 0; j < v.size(); ++j)
            row_result += m.at(i).at(j) * v.at(j);
        result.emplace_back(row_result);
    }
    return result;
}

vector mul(vector const &v, matrix const &m) {
    vector result;
    for (auto i = 0; i < m.at(0).size(); ++i) {
        double col_result = 0;
        for (auto j = 0; j < v.size(); ++j)
            col_result += v.at(j) * m.at(j).at(i);
        result.emplace_back(col_result);
    }
    return result;
}

vector sub(vector const &v, vector const &sub_v) {
    vector res;
    for (auto i = 0; i < v.size(); ++i)
        res.emplace_back(v.at(i) - sub_v.at(i));
    return res;
}

matrix inverseMatrixOf(matrix const &m) {
    auto size = m.size();
    matrix aug_matrix = m;
    vector v(size);
    for (auto i = 0; i < size; ++i) {
        v.at(i) = 1;
        aug_matrix.at(i).insert(std::end(aug_matrix.at(i)), std::begin(v), std::end(v));
        v.at(i) = 0;
    }
    while (true) {
        std::vector<bool> all;
        for (auto i = 0; i < size; ++i) {
            if (aug_matrix.at(i).at(i)) {
                all.emplace_back((bool)aug_matrix.at(i).at(i));
                continue;
            }
            for (auto j = 0; j < size; ++j) {
                if (j == i)
                    continue;
                if (aug_matrix.at(j).at(i) != 0 && aug_matrix.at(i).at(j) != 0) {
                    aug_matrix.at(j).swap(aug_matrix.at(i));
                    break;
                }
            }
            all.emplace_back((bool)aug_matrix.at(i).at(i));
        }
        if (std::all_of(all.begin(), all.end(), [](bool it) { return it; } ))
             break;
        all.clear();
    }
    for (auto i = 0; i < size; ++i) {
        auto d_el = aug_matrix.at(i).at(i);
        for (auto j = 0; j < size; ++j) {
            if (j == i)
                continue;
            auto x = -1 * aug_matrix.at(j).at(i) / d_el;
            auto new_row = mul(aug_matrix.at(i), x);
            addInPlace(aug_matrix.at(j), new_row);
        }
        mulInPlace(aug_matrix.at(i), 1 / d_el);
    }
    matrix inversed;
    for (auto i = 0; i < size; ++i)
        inversed.emplace_back(vector(aug_matrix.at(i).begin() + size, aug_matrix.at(i).end()));
    return inversed;
}

matrix shermanMorrison(matrix const &a_b, vector const &d, unsigned long s) {
    auto n = a_b.size();
    matrix inversed(n, vector(n));
    auto l = d;
    auto coff = -1 / l.at(s);
    l.at(s) = -1;
    for (auto &e: l)
        e *= coff;
    for (auto k = 0; k < n; k++) {
        for (auto j = 0; j < n; j++) {
            if (k == s)
                inversed.at(k).at(j) = l.at(k) * a_b.at(k).at(j);
            else
                inversed.at(k).at(j) = a_b.at(k).at(j) + l.at(k) * a_b.at(s).at(j);
        }
    }
    return inversed;
}

void fillInIndexes(vector &j, vector &j_b, vector &j_n) {
    j_n.clear();
    auto j_b_cp = j_b;
    std::sort(j_b_cp.begin(), j_b_cp.end());
    std::generate(j.begin(), j.end(), [i = 0]() mutable { return i++; });
    std::set_difference(j.begin(), j.end(), j_b_cp.begin(), j_b_cp.end(), std::inserter(j_n, j_n.begin()));
}

void mainPhase(int m, int n, matrix &a, vector &b, vector &c, vector &x, vector &j_b) {
    for (auto &e: j_b) --e;
    vector j(n), j_n;
    matrix a_b_inv;
    vector z;
    unsigned long s = 0;
    for (auto i = 0; ; ++i) {
        fillInIndexes(j, j_b, j_n);
        auto a_b = basisMatrixFor(a, j_b);
        if (i == 0)
            a_b_inv = inverseMatrixOf(a_b);
        else
            a_b_inv = shermanMorrison(a_b_inv, z, s);
        vector c_b;
        for (auto &basis_index: j_b)
            c_b.emplace_back(c.at(basis_index));
        auto u = mul(c_b, a_b_inv);
        auto delta = sub(mul(u, a), c);
        std::vector<bool> check;
        for (auto i = 0; i < delta.size(); ++i) {
            if (std::find(j_n.begin(), j_n.end(), i) != j_n.end()) {
                if (delta.at(i) >= 0)
                    check.emplace_back(true);
                else
                    check.emplace_back(false);
            }
        }
        if (std::all_of(check.begin(), check.end(), [](auto e) { return e; })) {
            endlPrint("Bounded");
            outputFor(x);
            return;
        }
        check.clear();
        auto j0 = std::min_element(delta.begin(), delta.end()) - delta.begin();
        auto a_j0 = columnFrom(a, (int)j0);
        z = mul(a_b_inv, a_j0);
        if (std::all_of(z.begin(), z.end(), [](auto e) { return e <= 0; })) {
            endlPrint("Unbounded");
            return;
        }
        vector teta;
        for (auto j = 0; j < m; ++j) {
            if (z.at(j) > 0)
                teta.emplace_back(x.at(j_b.at(j)) / z.at(j));
            else
                teta.emplace_back(std::numeric_limits<double>::infinity());
        }
        s = std::min_element(teta.begin(), teta.end()) - teta.begin();
        auto t0 = teta.at(s);
        for (auto i = 0; i < m; ++i)
            x.at(j_b.at(i)) -= t0 * z.at(i);
        for (auto const &j_n_i: j_n)
            x.at(j_n_i) = 0;
        x.at(j0) = t0;
        j_b.at(s) = j0;
    }
}

int main(int argc, const char * argv[]) {
    int n, m;
    std::cin >> m >> n;
    matrix a(m, vector(n));
    vector b(m);
    vector c(n);
    vector x(n);
    vector j_b(m);
    inputFor(a);
    inputFor(b);
    inputFor(c);
    inputFor(x);
    inputFor(j_b);
    mainPhase(m, n, a, b, c, x, j_b);
    return 0;
}
