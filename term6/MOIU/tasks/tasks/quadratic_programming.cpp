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

void outputFor(matrix const &m) {
    for (unsigned long i = 0; i < m.size(); ++i) {
        for (unsigned long j = 0; j < m.at(0).size(); ++j) {
            answerPrint(m.at(i).at(j));
        }
        std::cout << "\n";
    }
}

void outputFor(vector const &v) {
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
    for (auto i = 0; i < m.at(0).size(); ++i) {
        double col_result = 0;
        for (auto j = 0; j < v.size(); ++j)
            col_result += v.at(j) * m.at(j).at(i);
        result.at(i) = col_result;
    }
    return result;
}

void addInPlace(vector &row, vector &added_row) {
    for (unsigned long j = 0; j < row.size(); ++j)
        row.at(j) += added_row.at(j);
}

vector columnFrom(matrix const &m, int index) {
    vector column(m.size());
    for (unsigned long i = 0; i < m.size(); ++i)
        column.at(i) = m.at(i).at(index);
    return column;
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
        if (std::abs(d_el) < EPSILON) {
            for (auto j = i + 1; j < size; ++j) {
                if (std::abs(aug_matrix.at(j).at(i)) > EPSILON) {
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

void quadraticProgramming(matrix const &a, vector const &b, vector &c, matrix const &d,
                          vector &x, indexes &j_op, indexes &j_extd) {
    
    bool skipFirstSteps = false;
    int j0 = -1;
    long double delta_j0 = 0.0;
    vector temp;
    matrix a_op_inv;
    
    while (true) {
        matrix a_op(a.size(), vector(j_op.size()));
        vector c_x_op(j_op.size());
        
        if (!skipFirstSteps) {
            
            // 1
            auto c_x = mul(d, x);
            addInPlace(c_x, c);
            
            // 2
            for (unsigned long i = 0; i < j_op.size(); ++i) {
                temp = columnFrom(a, j_op.at(i));
                for (unsigned long j = 0; j < temp.size(); ++j)
                    a_op.at(j).at(i) = temp.at(j);
                c_x_op.at(i) = c_x.at(j_op.at(i));
            }
            
            // 3
            mulInPlace(c_x_op, -1);
            a_op_inv = inverseMatrixOf(a_op);
            auto u = mul(c_x_op, a_op_inv);
            
            j0 = -1;
            delta_j0 = -EPSILON;
            for (int i = 0; i < a.at(0).size(); ++i) {
                if (std::find(j_extd.begin(), j_extd.end(), i) != j_extd.end())
                    continue;
                auto delta = 0.0;
                for (int j = 0; j < u.size(); ++j)
                    delta += u.at(j) * a.at(j).at(i);
                delta += c_x.at(i);
                if (delta >= delta_j0)
                    continue;
                delta_j0 = delta;
                j0 = i;
            }
            if (j0 == -1) {
                endlPrint("Bounded");
                outputFor(x);
                return;
            }
        }
        
        matrix h(j_extd.size() + a.size(), vector(j_extd.size() + a.size()));
        matrix a_star(a.size(), vector(j_extd.size()));
        vector bb(j_extd.size() + a.size());
        
        // 4
        for (unsigned long i = 0; i < j_extd.size(); ++i) {
            temp = columnFrom(a, j_extd.at(i));
            for (unsigned long j = 0; j < temp.size(); ++j)
                a_star.at(j).at(i) = temp.at(j);
        }
        
        vector l(a.at(0).size());
        l.at(j0) = 1;
        
        for (int i = 0; i < j_extd.size(); ++i) {
            int j = 0;
            for (; j < j_extd.size(); ++j)
                h.at(i).at(j) = d.at(j_extd.at(i)).at(j_extd.at(j));
            for (; j < j_extd.size() + a.size(); ++j)
                h.at(i).at(j) = a_star.at(j - j_extd.size()).at(i);
        }
        for (unsigned long i = j_extd.size(); i < a.size() + j_extd.size(); ++i) {
            int j = 0;
            for (; j < a_star.at(0).size(); ++j)
                h.at(i).at(j) = a_star.at(i - j_extd.size()).at(j);
            for (; j < j_extd.size() + a.size() - a_star.at(0).size(); ++j)
                h.at(i).at(j) = 0;
        }
        
        for (unsigned long i = 0; i < j_extd.size(); ++i)
            bb.at(i) = d.at(j_extd.at(i)).at(j0);
        for (unsigned long i = j_extd.size(); i < j_extd.size() + a.size(); ++i)
            bb.at(i) = a.at(i - j_extd.size()).at(j0);
        
        auto h_inv = inverseMatrixOf(h);
        auto l_y = mul(h_inv, bb);
        mulInPlace(l_y, -1);
        for (unsigned long i = 0; i < j_extd.size(); ++i)
            l.at(j_extd.at(i)) = l_y.at(i);
        
        // 5
        auto theta_j_min = std::numeric_limits<long double>::infinity();
        long double l_i = 0.0;
        long double theta_i = 0.0;
        
        int j_star = -1;
        int s = -1;
        
        for (unsigned long i = 0; i < j_extd.size(); ++i) {
            l_i = l.at(j_extd.at(i));
            if (l_i > -EPSILON)
                continue;
            theta_i = -x.at(j_extd.at(i)) / l_i;
            if (theta_i >= theta_j_min)
                continue;
            j_star = j_extd.at(i);
            theta_j_min = theta_i;
            s = (int)i;
        }
        
        auto theta_j0 = std::numeric_limits<long double>::infinity();
        long double delta = 0.0;
        for (unsigned long i = 0; i < j_extd.size(); ++i)
            delta += d.at(j_extd.at(i)).at(j0) * l_y.at(i);
        auto a_j0 = columnFrom(a, j0);
        for (unsigned long i = 0; i < l_y.size() - j_extd.size(); ++i)
            delta += a_j0.at(i) * l_y.at(i + j_extd.size());
        delta += d.at(j0).at(j0);
        if (delta > EPSILON)
            theta_j0 = std::abs(delta_j0) / delta;
        
        auto theta0 = std::min(theta_j_min, theta_j0);
        if (theta0 == std::numeric_limits<long double>::infinity()) {
            endlPrint("Unbounded");
            return;
        }
        
        // 6
        mulInPlace(l, theta0);
        addInPlace(x, l);
        
        // 7
        if (theta0 == theta_j0) {
            j_star = j0;
            j_extd.emplace_back(j0);
            std::sort(j_extd.begin(), j_extd.end());
            skipFirstSteps = false;
        } else {
            auto index = std::find(j_op.begin(), j_op.end(), j_star) - j_op.begin();
            if (index < j_op.size()) {
                j_star = j_op.at(index);
                vector e(a_op_inv.size());
                e.at(index) = 1;
                auto j_plus = -1;
                for (int i = 0; i < j_extd.size(); ++i) {
                    if (std::find(j_op.begin(), j_op.end(), j_extd.at(i)) != j_op.end())
                        continue;
                    auto e_s_a_op = mul(e, a_op_inv);
                    auto col = columnFrom(a, j_extd.at(i));
                    long double res = 0.0;
                    for (int j = 0; j < a_op_inv.size(); ++j)
                        res += e_s_a_op.at(j) * col.at(j);
                    if (std::abs(res) < EPSILON)
                        continue;
                    j_plus = j_extd.at(i);
                    break;
                }
                if (j_plus == -1) {
                    j_op.erase(j_op.begin() + index);
                    j_op.emplace_back(j0);
                    index = std::find(j_extd.begin(), j_extd.end(), j_star) - j_extd.begin();
                    j_extd.erase(j_extd.begin() + index);
                    j_extd.emplace_back(j0);
                    std::sort(j_op.begin(), j_op.end());
                    std::sort(j_extd.begin(), j_extd.end());
                    skipFirstSteps = false;
                } else {
                    j_op.erase(j_op.begin() + index);
                    j_op.emplace_back(j_plus);
                    index = std::find(j_extd.begin(), j_extd.end(), j_star) - j_extd.begin();
                    j_extd.erase(j_extd.begin() + index);
                    std::sort(j_op.begin(), j_op.end());
                    delta_j0 += theta0 * delta;
                    skipFirstSteps = true;
                }
            } else {
                index = std::find(j_extd.begin(), j_extd.end(), j_star) - j_extd.begin();
                j_extd.erase(j_extd.begin() + index);
                delta_j0 += theta0 * delta;
                skipFirstSteps = true;
            }
        }
    }
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
    quadraticProgramming(a, b, c, d, x, j_op, j_extd);
    return 0;
}
