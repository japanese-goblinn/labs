//
//  simplex_method.cpp
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
    for (unsigned long i = 0; i < matrix.size(); ++i)
        for (unsigned long j = 0; j < matrix.at(0).size(); ++j)
            std::cin >> matrix.at(i).at(j);
}

void inputFor(vector &vector) {
    for (unsigned long i = 0; i < vector.size(); ++i)
        std::cin >> vector.at(i);
}

vector columnFrom(matrix const &m, int index) {
    vector column(m.size());
    for (unsigned long i = 0; i < m.size(); ++i)
        column[i] = m[i][index];
    return column;
}

void mulInPlace(vector &v, double value) {
    for (unsigned long i = 0; i < v.size(); ++i)
        v.at(i) *= value;
}

void addInPlace(vector &row, vector const &added_row) {
    for (unsigned long j = 0; j < row.size(); ++j)
        row.at(j) += added_row.at(j);
}

vector mul(vector const &v, double value) {
    vector nv(v.size());
    for (unsigned long i = 0; i < v.size(); ++i)
        nv.at(i) = value * v.at(i);
    return nv;
}

vector mul(matrix const &m, vector const &v) {
    vector result(m.size());
    for (unsigned long i = 0; i < m.size(); ++i) {
        auto row_result = 0.0;
        for (unsigned long j = 0; j < m.at(0).size(); ++j)
            row_result += m.at(i).at(j) * v.at(j);
        result.at(i) = row_result;
    }
    return result;
}

vector mul(vector const &v, matrix const &m) {
    vector result(m.at(0).size());
    for (unsigned long i = 0; i < m.at(0).size(); ++i) {
        auto col_result = 0.0;
        for (unsigned long j = 0; j < v.size(); ++j)
            col_result += v.at(j) * m.at(j).at(i);
        result.at(i) = col_result;
    }
    return result;
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
    while (true) {
        std::vector<bool> all;
        for (unsigned long i = 0; i < size; ++i) {
            if (aug_matrix.at(i).at(i)) {
                all.emplace_back((bool)aug_matrix.at(i).at(i));
                continue;
            }
            for (unsigned long j = 0; j < size; ++j) {
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
    for (unsigned long i = 0; i < size; ++i) {
        auto d_el = aug_matrix.at(i).at(i);
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

matrix shermanMorrison(matrix const &a_b, vector const &d, unsigned long s) {
    auto n = a_b.size();
    matrix inversed(n, vector(n));
    auto l = d;
    auto coff = -1 / l[s];
    l[s] = -1;
    for (unsigned long i = 0; i < l.size(); ++i)
        l[i] *= coff;
    for (unsigned long k = 0; k < n; k++) {
        for (unsigned long j = 0; j < n; j++) {
            if (k == s)
                inversed[k][j] = l[k] * a_b[k][j];
            else
                inversed[k][j] = a_b[k][j] + l[k] * a_b[s][j];
        }
    }
    return inversed;
}

void mainPhase(int m, int n, matrix &a, matrix &a_b_inv,
                vector &b, vector &c, vector &x, std::vector<int> &j_b, bool need_output = true) {
    vector c_b(j_b.size());
    for (auto i = 0; i < j_b.size(); ++i)
        c_b.at(i) = c.at(j_b.at(i));
    while (true) {
        auto u = mul(c_b, a_b_inv);
        auto j_0 = INT_MIN;
        auto delta_j0 = 0.0;
        for (auto i = 0; i < n; ++i) {
            if (std::find(j_b.begin(), j_b.end(), i) != j_b.end())
                continue;
            auto colum_res = 0.0;
            for (int j = 0; j < j_b.size(); ++j)
                colum_res += u.at(j) * a.at(j).at(i);
            colum_res -= c.at(i);
            if (colum_res < delta_j0) {
                delta_j0 = colum_res;
                j_0 = i;
            }
        }
        if (j_0 == INT_MIN) {
            if (need_output) {
                endlPrint("Bounded");
                outputFor(x);
            }
            return;
        }
        auto a_j0 = columnFrom(a, j_0);
        auto z = mul(a_b_inv, a_j0);
        if (std::all_of(z.begin(), z.end(), [](auto e) { return e < EPSILON; })) {
            if (need_output)
                endlPrint("Unbounded");
            exit(0);
        }
        auto teta_0 = std::numeric_limits<double>::infinity();
        auto s = -1;
        for (auto j = 0; j < j_b.size(); ++j) {
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
        c_b.at(s) = c.at(j_0);
        a_b_inv = shermanMorrison(a_b_inv, z, s);
    }
}

void fillFakeIndexesFromBasis(std::vector<int> &fake, std::vector<int> const &j_b, int n) {
    if (!fake.empty())
        fake.clear();
    for (unsigned long i = 0; i < j_b.size(); ++i) {
        if (j_b[i] < n)
            continue;
        fake.emplace_back(j_b[i]);
    }
}

void firstPhase(int &m, int &n, matrix &a_star, matrix &a_b_inv, vector &b, vector const &c,
                vector &x_star, std::vector<int> &j_b) {
    for (auto i = 0; i < m; ++i) {
        if (b[i] >= 0)
            continue;
        mulInPlace(a_star[i], -1);
        b[i] *= - 1;
    }
    matrix a_b;
    a_b.reserve(m);
    vector v(m);
    for (auto i = 0; i < m; ++i) {
        v[i] = 1;
        a_star[i].insert(a_star[i].end(), v.begin(), v.end());
        a_b.emplace_back(v);
        v[i] = 0;
    }
    a_b_inv = inverseMatrixOf(a_b);
    x_star.insert(x_star.end(), b.begin(), b.end());
    vector c_star(n);
    const vector e(m, -1);
    c_star.insert(c_star.end(), e.begin(), e.end());
    std::vector<int> j_u(m);
    std::generate(j_u.begin(), j_u.end(), [e = n]() mutable { return e++; });
    j_b = j_u;
    mainPhase(m, m + n, a_star, a_b_inv, b, c_star, x_star, j_b, false);
    if (std::any_of(x_star.begin() + n, x_star.end(), [](auto e) { return std::abs(e) > EPSILON; })) {
        endlPrint("No solution");
        exit(0);
    }
    std::vector<int> fake_in_basis_indexes;
    while (true) {
        fillFakeIndexesFromBasis(fake_in_basis_indexes, j_b, n);
        if (fake_in_basis_indexes.empty()) {
            x_star = vector(x_star.begin(), x_star.end() - m);
            return;
        }
        for (auto const &j_k: fake_in_basis_indexes) {
            auto i_0 = j_k - n;
            auto all_zeros = true;
            for (auto j_i = 0; j_i < n; ++j_i) {
                if (!a_star[i_0][j_i])
                    continue;
                auto a_j = columnFrom(a_star, j_i);
                vector l(m);
                for (unsigned long i = 0; i < a_b_inv.size(); ++i) {
                    auto row_result = 0.0;
                    for (unsigned long j = 0; j < a_b_inv.size(); ++j)
                        row_result += a_b_inv[i][j] * v[j];
                    l[i] = row_result;
                }
                if (std::abs(l[i_0]) > EPSILON) {
                    auto replace_index = std::find(j_b.begin(), j_b.end(), j_k) - j_b.begin();
                    j_b[replace_index] = j_i;
                    vector r_c(a_b_inv.size());
                    for (unsigned long i = 0; i < a_b_inv.size(); ++i)
                        r_c[i] = a_b_inv[i][j_i];
                    a_b_inv = shermanMorrison(a_b_inv, r_c, replace_index);
                    all_zeros = false;
                }
            }
            if (all_zeros) {
                auto replace_index = std::find(j_b.begin(), j_b.end(), j_k) - j_b.begin();
                j_b.erase(j_b.begin() + replace_index);
                a_b_inv.erase(a_b_inv.begin() + replace_index);
                for (unsigned long i = 0; i < a_b_inv.size(); ++i)
                    a_b_inv[i].erase(a_b_inv[i].begin() + i_0);
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n, m;
    std::cin >> m >> n;
    matrix a(m, vector(n));
    vector b(m);
    vector c(n);
    inputFor(a);
    inputFor(b);
    inputFor(c);
    vector x(n);
    std::vector<int> j_b;
    matrix a_inv(m, vector(n));
    firstPhase(m, n, a, a_inv, b, c, x, j_b);
    mainPhase(m, n, a, a_inv, b, c, x, j_b);
    return 0;
}
