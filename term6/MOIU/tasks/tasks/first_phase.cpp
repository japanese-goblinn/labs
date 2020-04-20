//
//  first_phase.cpp
//  tasks
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

#define EPSILON 1e-10
#define endlPrint(x) std::cout << x << "\n"
#define answerPrint(x) std::cout.precision(15); std::cout << std::fixed << x << " ";

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

void outputFor(matrix &m) {
    auto n = m.at(0).size();
    for (auto i = 0; i < m.size(); ++i) {
        for (auto j = 0; j < n; ++j) {
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
    result.reserve(m.size());
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
    result.reserve(m.size());
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
    res.reserve(v.size());
    for (auto i = 0; i < v.size(); ++i)
        res.emplace_back(v.at(i) - sub_v.at(i));
    return res;
}

matrix augMatrixFor(matrix const &m) {
    auto size = m.size();
    matrix aug_matrix = m;
    vector v(size);
    for (auto i = 0; i < size; ++i) {
        v.at(i) = 1;
        aug_matrix.at(i).insert(aug_matrix.at(i).end(), v.begin(), v.end());
        v.at(i) = 0;
    }
    return aug_matrix;
}

matrix inverseMatrixOf(matrix const &m) {
    auto size = m.size();
    matrix aug_matrix = augMatrixFor(m);
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
                if (aug_matrix.at(j).at(i) != 0) {
                    aug_matrix.at(j).swap(aug_matrix.at(i));
                    break;
                }
            }
            all.emplace_back((bool)aug_matrix.at(i).at(i));
        }
        if (std::all_of(all.begin(), all.end(), [](bool it) { return it; } ))
             break;
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


matrix mainPhase(int m, int n, matrix &a, matrix const &a_b, vector &b, vector &c, vector &x, vector &j_b) {
    matrix a_b_inv;
    a_b_inv = inverseMatrixOf(a_b);
    vector c_b(m);
    while (true) {
        for (auto i = 0; i < m; ++i)
            c_b.at(i) = c.at(j_b.at(i));
        auto u = mul(c_b, a_b_inv);
        auto j_0 = INT_MIN;
        auto delta_j0 = 0.0;
        for (int i = 0; i < n; ++i) {
            if (std::find(j_b.begin(), j_b.end(), i) != j_b.end())
                continue;
            auto colum_res = 0.0;
            for (int j = 0; j < m; ++j)
                colum_res += u.at(j) * a.at(j).at(i);
            colum_res -= c.at(i);
            if (colum_res < delta_j0) {
                delta_j0 = colum_res;
                j_0 = i;
            }
        }
        if (j_0 == INT_MIN) {
//            endlPrint("Bounded");
//            outputFor(x);
            return a_b_inv;
        }
        auto a_j0 = columnFrom(a, j_0);
        auto z = mul(a_b_inv, a_j0);
        if (std::all_of(z.begin(), z.end(), [](auto e) { return e <= 0; })) {
//            endlPrint("Unbounded");
            return a_b_inv;
        }
        auto teta_0 = std::numeric_limits<double>::infinity();
        auto s = -1;
        for (auto j = 0; j < m; ++j) {
            if (z.at(j) <= 0)
                continue;
            auto teta_j = x.at(j_b.at(j)) / z.at(j);
            if (teta_j < teta_0) {
                teta_0 = teta_j;
                s = j;
            }
        }
        for (auto i = 0; i < n; ++i) {
            auto j_b_index = std::find(j_b.begin(), j_b.end(), i);
            if (j_b_index == j_b.end()) {
                x.at(i) = 0;
                continue;
            }
            auto j = j_b_index - j_b.begin();
            x.at(i) -= teta_0 * z.at(j);
        }
        x.at(j_0) = teta_0;
        j_b.at(s) = j_0;
        a_b_inv = shermanMorrison(a_b_inv, z, s);
    }
}

vector fakeIndexesFromBasis(vector const &j_b, int n) {
    vector fake;
    for (auto i = 0; i < j_b.size(); ++i) {
        if (j_b.at(i) < n)
            continue;
        fake.emplace_back(j_b.at(i));
    }
    return fake;
}

void firstPhase(int m, int n, matrix &a, vector &b, vector const &c) {
    for (auto i = 0; i < m; ++i) {
        if (b.at(i) >= 0)
            continue;
        mulInPlace(a.at(i), -1);
        b.at(i) *= - 1;
    }
    matrix a_star = a;
    matrix a_b;
    a_b.reserve(m);
    vector v(m);
    for (auto i = 0; i < m; ++i) {
        v.at(i) = 1;
        a_star.at(i).insert(a_star.at(i).end(), v.begin(), v.end());
        a_b.emplace_back(v);
        v.at(i) = 0;
    }
    vector x_star(n);
    x_star.insert(x_star.end(), b.begin(), b.end());
    vector c_star(n);
    const vector e(m, -1);
    c_star.insert(c_star.end(), e.begin(), e.end());
    vector j_u(m);
    std::generate(j_u.begin(), j_u.end(), [e = n]() mutable { return e++; });
    vector j_b = j_u;
    auto a_b_inv = mainPhase(m, m + n, a_star, a_b, b, c_star, x_star, j_b);
    if (std::any_of(x_star.begin() + n, x_star.end(), [](auto e) { return std::abs(e) > EPSILON; })) {
        endlPrint("No solution");
        return;
    }
    while (true) {
        auto fake_in_basis_indexes = fakeIndexesFromBasis(j_b, n);
        if (fake_in_basis_indexes.empty()) {
            endlPrint("Bounded");
            auto x = vector(x_star.begin(), x_star.end() - m);
            outputFor(x);
            return;
        }
        for (auto const &j_k: fake_in_basis_indexes) {
            auto i_0 = j_k - n;
            auto all_zeros = true;
            for (auto j_i = 0; j_i < n; ++j_i) {
                if (!a_star.at(i_0).at(j_i))
                    continue;
                auto a_j = columnFrom(a_star, j_i);
                auto l = mul(a_b_inv, a_j);
                if (std::abs(l.at(i_0)) > EPSILON) {
                    auto replace_index = std::find(j_b.begin(), j_b.end(), j_k) - j_b.begin();
                    j_b.at(replace_index) = j_i;
                    a_b_inv = shermanMorrison(a_b_inv, a_j, replace_index);
                    all_zeros = false;
                }
            }
            if (all_zeros) {
                auto replace_index = std::find(j_b.begin(), j_b.end(), j_k) - j_b.begin();
                j_b.erase(j_b.begin() + replace_index);
                a_b_inv.erase(a_b_inv.begin() + replace_index);
                for (auto &row: a_b_inv)
                    row.erase(row.begin() + i_0);
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    int n, m;
    std::cin >> m >> n;
    matrix a(m, vector(n));
    vector b(m);
    vector c(n);
    inputFor(a);
    inputFor(b);
    inputFor(c);
    firstPhase(m, n, a, b, c);
    return 0;
}
