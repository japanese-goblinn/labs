//
//  sherman_morrison.cpp
//  tasks
//
//  Created by Kirill Gorbachyonok on 4/1/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>

typedef std::vector<double> vector;
typedef std::vector<std::vector<double>> matrix;

#define endl_print(x) std::cout << x << std::endl
#define answer_print(x) std::cout.precision(15); std::cout << std::fixed << x << " ";

vector multiplication(matrix const &m, vector const &v) {
    vector result(v.size());
    for (int i = 0; i < v.size(); i++) {
        double raw_result = 0;
        for (int j = 0; j < v.size(); j++) {
            raw_result += m[i][j] * v[j];
        }
        result[i] = raw_result;
    }
    return result;
}

void input_for(matrix &m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> m[i][j];
        }
    }
}

void output_for(matrix &m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            answer_print(m[i][j]);
        }
        std::cout << std::endl;
    }
}

matrix sherman_morrison(int n, int i, matrix const &b, vector const &x) {
    matrix inversed(n, vector(n));
    auto l = multiplication(b, x);
    if (l[i] == 0) {
        endl_print("NO");
        return inversed;
    }
    else {
        endl_print("YES");
    }
    auto coff = -1 / l[i];
    l[i] = -1;
    for (auto &e: l)
        e *= coff;
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            if (k == i)
                inversed[k][j] = l[k] * b[k][j];
            else
                inversed[k][j] = b[k][j] + l[k] * b[i][j];
        }
    }
    return inversed;;
}

int main(int argc, const char * argv[]) {
    int n, i;
    std::cin >> n >> i;
    matrix a(n, vector(n));
    matrix b(n, vector(n));
    vector x(n);
    input_for(a, n);
    input_for(b, n);
    for (int i = 0; i < n; i++) {
        std::cin >> x[i];
    }
    auto inv = sherman_morrison(n, i - 1, b, x);
    output_for(inv, n);
    return 0;
}
