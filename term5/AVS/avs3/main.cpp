#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"
#include <iostream>
#include <vector>
#include "omp.h"


int main() {
    alignas(64) const int n = 2056, m = 2056;
//    std::cout << "Please, enter matrix dimensions: ";
//    std::cin >> n >> m;
    alignas(64) std::vector<std::vector<int>> matrix(n, std::vector<int>(m, 1));
    alignas(64) std::vector<int> vector(m, 1);
    alignas(64) std::vector<int> result(n, 0);

    auto start_p = omp_get_wtime();
    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < n; i++) {
        int row_result = 0;
        for (int j = 0; j < m; j++) {
            row_result += matrix[i][j] * vector[j];
        }
        result[i] = row_result;
    }
    auto end_p = omp_get_wtime();
    auto res_p = end_p - start_p;
    std::cout << "Parallel time: " << res_p << std::endl;

//    for (auto &item: result) {
//        std::cout << item << " ";
//    }

    std::fill(result.begin(), result.end(), 0);

    auto start_s = omp_get_wtime();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    auto end_s = omp_get_wtime();
    auto res_s = end_s - start_s;
    std::cout << "Not parallel time: " << res_s << std::endl;

    std::cout << "Speedup: " << res_s / res_p << std::endl;

    return 0;
}
#pragma clang diagnostic pop