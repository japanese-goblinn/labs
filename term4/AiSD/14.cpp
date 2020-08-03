#include <iostream>
#include <vector>

long long n;
long long mod = (long long)1e9 + 7;

std::vector<std::vector<long long>> matrix_mul(std::vector<std::vector<long long>> &first_matrix, std::vector<std::vector<long long>> second_matrix) {
    std::vector<std::vector<long long>> result(n, std::vector<long long>(n, 0));
    for (long long i = 0; i < first_matrix.size(); i++) {
        for (long long j = 0; j < first_matrix[i].size(); j++) {
            for (long long k = 0; k < n; k++) {
                result[i][j] += (first_matrix[i][k] * second_matrix[k][j]) % mod;
                result[i][j] %= mod;
            }
        }
    }
    return result;
}

std::vector<std::vector<long long>> binary_pow(std::vector<std::vector<long long>> &matrix, long long k, std::vector<std::vector<long long>> &ed_matrix) {
    if (!k) {
        return ed_matrix;
    }
    if (k % 2) {
        std::vector<std::vector<long long>> second_matrix = binary_pow(matrix, k - 1, ed_matrix);
        return matrix_mul(second_matrix, matrix);
    }
    else {
        std::vector<std::vector<long long>> second_matrix = binary_pow(matrix, k / 2, ed_matrix);
        return matrix_mul(second_matrix, second_matrix);
    }
}

int main(int argc, const char * argv[]) {
    long long m, u, v, length, a, b;
    std::cin >> n >> m;
    std::vector<std::vector<long long>> ed_matrix(n, std::vector<long long>(n, 0));
    for (long long i = 0; i < n; i++) {
        ed_matrix[i][i] = 1;
    }
    std::vector<std::vector<long long>> graph(n, std::vector<long long>(n, 0));
    std::cin >> u >> v >> length;
    for (long long k = 0; k < m; k++) {
        std::cin >> a >> b;
        graph[a - 1][b - 1] += 1;
        graph[b - 1][a - 1] += 1;
    }
    std::vector<std::vector<long long>> result = binary_pow(graph, length, ed_matrix);
    std::cout << result[u - 1][v - 1] % mod;
