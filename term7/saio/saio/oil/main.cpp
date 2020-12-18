//
//  main.cpp
//  oil
//
//  Created by Kirill on 12/16/20.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>
#include <cmath>
#include <numeric>
#include <queue>


typedef std::pair<int, int> coordinate;
typedef std::pair<int, coordinate> cell;
typedef std::vector<int> vector;
typedef std::vector<std::vector<int>> matrix;

#define endlPrint(x) std::cout << x << "\n"
#define answerPrint(x) std::cout << x << " ";

void inputFor(std::ifstream& f, matrix &matrix) {
    for (unsigned long i = 0; i < matrix.size(); ++i)
        for (unsigned long j = 0; j < matrix.at(0).size(); ++j)
            f >> matrix.at(i).at(j);
}

void inputFor(std::ifstream& f, vector &vector) {
    for (unsigned long i = 0; i < vector.size(); ++i)
        f >> vector.at(i);
}

void addColumn(matrix &m, int value = 0) {
    for (auto &row : m)
        row.emplace_back(value);
}

void addRow(matrix &m, int value = 0) {
    m.emplace_back(vector(m.at(0).size(), value));
}

vector getBasisCellsIndexesForColumn(unsigned long columnIndex, std::vector<cell> const &basis) {
    vector indexes;
    for (unsigned long i = 0; i < basis.size(); ++i) {
        if (basis.at(i).second.second != columnIndex)
            continue;
        indexes.emplace_back(i);
    }
    return indexes;
}

std::vector<cell> getBasisCellsForColumn(unsigned long columnIndex, std::vector<cell> const &basis) {
    std::vector<cell> cells;
    for (unsigned long i = 0; i < basis.size(); ++i) {
        if (basis.at(i).second.second != columnIndex)
            continue;
        cells.emplace_back(basis.at(i));
    }
    return cells;
}

vector getBasisCellsIndexesForRow(unsigned long rowIndex, std::vector<cell> const &basis) {
    vector indexes;
    for (unsigned long i = 0; i < basis.size(); ++i) {
        if (basis.at(i).second.first != rowIndex)
            continue;
        indexes.emplace_back(i);
    }
    return indexes;
}

std::vector<cell> getBasisCellsForRow(unsigned long rowIndex, std::vector<cell> const &basis) {
    std::vector<cell> cells;
    for (unsigned long i = 0; i < basis.size(); ++i) {
        if (basis.at(i).second.first != rowIndex)
            continue;
        cells.emplace_back(basis.at(i));
    }
    return cells;
}

//void balance(matrix &x, matrix &c, vector &a, vector &b) {
//    auto stocks_sum = std::accumulate(a.begin(), a.end(), 0);
//    auto stores_sum = std::accumulate(b.begin(), b.end(), 0);
//    if (stocks_sum < stores_sum) {
//        a.emplace_back(stores_sum - stocks_sum);
//        addRow(c);
//        addRow(x);
//    }
//    if (stocks_sum > stores_sum) {
//        b.emplace_back(stocks_sum - stores_sum);
//        addColumn(c);
//        addColumn(x);
//    }
//}

void potentialsCalculation(matrix &c, std::vector<cell> &basis, vector &v, vector &u, unsigned long n) {
    std::queue<int> q;
    std::vector<bool> alreadyCounted(basis.size());

    auto indexes = getBasisCellsIndexesForColumn(n - 1, basis);
    auto i = 0;
    auto j = 0;
    for (auto const &index : indexes) {
        i = basis.at(index).second.first;
        j = basis.at(index).second.second;
        alreadyCounted.at(index) = true;
        u.at(i) = c.at(i).at(j);
        q.push(i);
    }

    auto isColumn = false;
    auto changeSize = q.size();

    while (!q.empty()) {
        if (isColumn) {
            while (changeSize--) {
                auto col = q.front();
                q.pop();
                indexes = getBasisCellsIndexesForColumn(col, basis);
                for (auto const &index: indexes) {
                    if (alreadyCounted.at(index))
                        continue;
                    alreadyCounted.at(index) = true;
                    i = basis.at(index).second.first;
                    j = basis.at(index).second.second;
                    u.at(i) = c.at(i).at(j) - v.at(j);
                    q.push(i);
                }
            }
        } else {
            while (changeSize--) {
                auto row = q.front();
                q.pop();
                indexes = getBasisCellsIndexesForRow(row, basis);
                for (auto const &index: indexes) {
                    if (alreadyCounted.at(index))
                        continue;
                    alreadyCounted.at(index) = true;
                    i = basis.at(index).second.first;
                    j = basis.at(index).second.second;
                    v.at(j) = c.at(i).at(j) - u.at(i);
                    q.push(j);
                }
            }
        }
        changeSize = q.size();
        isColumn = !isColumn;
    }
}

cell findNonBasisMinimum(matrix &c, std::vector<cell> &basis, vector &v, vector &u) {
    coordinate min;
    int minCost = 0;
    int delta;
    for (int i = 0; i < c.size(); ++i) {
        for (int j = 0; j < c.at(0).size(); ++j) {
            delta = c.at(i).at(j) - u.at(i) - v.at(j);
            if (delta >= minCost)
                continue;
            minCost = delta;
            min = { i, j };
        }
    }
    return cell(minCost, min);
}

std::vector<cell> deletingMethod(std::vector<cell> const &basis, unsigned long n, unsigned long m) {
    std::vector<cell> cycleCells;
    std::vector<long> iCheck(m);
    std::vector<long> jCheck(n);
    
    for (int i = 0; i < m; ++i)
        iCheck.at(i) = getBasisCellsIndexesForRow(i, basis).size();
    for (int j = 0; j < n; ++j)
        jCheck.at(j) = getBasisCellsIndexesForColumn(j, basis).size();
    
    auto i = 0;
    auto j = 0;
    auto findedOne = true;
    while (findedOne) {
        findedOne = false;
        for (const auto &basisCell : basis) {
            i = basisCell.second.first;
            j = basisCell.second.second;
            if (iCheck.at(i) != 1 && jCheck.at(j) != 1)
                continue;
            if (jCheck.at(j) == 0)
                continue;
            if (iCheck.at(i) == 0)
                continue;
            iCheck.at(i)--;
            jCheck.at(j)--;
            findedOne = true;
        }
    }
    
    for (const auto &basisCell : basis) {
        i = basisCell.second.first;
        j = basisCell.second.second;
        if (iCheck.at(i) == 0 || jCheck.at(j) == 0)
            continue;
        cycleCells.emplace_back(basisCell);
    }
    return cycleCells;
}

void parseCycle(std::vector<cell> &cycle, cell minimum, matrix &x, std::vector<cell> &basis) {
    std::vector<cell> orderedCells;
    orderedCells.reserve(cycle.size());
    long count = cycle.size();
    
    auto currentCell = minimum;
    orderedCells.emplace_back(currentCell);
    count--;
    
    auto columGo = true;
    std::vector<cell> cells;
    
    while (count--) {
        if (columGo) {
            cells = getBasisCellsForColumn(currentCell.second.second, cycle);
            std::sort(cells.begin(), cells.end(), [](auto lhs, auto rhs) { return lhs.second.first < rhs.second.first; });
        } else {
            cells = getBasisCellsForRow(currentCell.second.first, cycle);
            std::sort(cells.begin(), cells.end(), [](auto lhs, auto rhs) { return lhs.second.second < rhs.second.second; });
        }
        for (int i = 0; i < cells.size(); ++i) {
            if (cells.at(i).second.first != currentCell.second.first || cells.at(i).second.second != currentCell.second.second)
                continue;
            if (i % 2) {
                currentCell = cells.at(i - 1);
            } else {
                currentCell = cells.at(i + 1);
            }
            orderedCells.emplace_back(currentCell);
            break;
        }
        columGo = !columGo;
    }
    
    int min = INT_MAX;
    coordinate minCoord;
    cell cell;
    for (int i = 1; i < orderedCells.size(); i += 2) {
        cell = orderedCells.at(i);
        if (x.at(cell.second.first).at(cell.second.second) >= min)
            continue;
        min = x.at(cell.second.first).at(cell.second.second);
        minCoord = { cell.second.first, cell.second.second };
    }
    for (int i = 0; i < orderedCells.size(); ++i) {
        cell = orderedCells.at(i);
        if (i % 2) {
            x.at(cell.second.first).at(cell.second.second) -= min;
        } else {
            x.at(cell.second.first).at(cell.second.second) += min;
        }
    }
    int deleteIndex = -1;
    for (int i = 0; i < basis.size(); ++i) {
        if (basis.at(i).second.first != minCoord.first || basis.at(i).second.second != minCoord.second)
            continue;
        deleteIndex = i;
    }
    basis.erase(basis.begin() + deleteIndex);
}

void matrixTransportTask(matrix &x, matrix &c, vector &a, vector &b) {
    auto n = x.at(0).size();
    auto m = x.size();
    auto storage = a;
    auto consumer = b;
    auto i = 0;
    auto j = 0;
    std::vector<cell> basis;
    basis.reserve(n + m - 1);
    while (j < n && i < m) {
        x.at(i).at(j) = std::min(storage.at(i), consumer.at(j));
        auto diff = storage.at(i) - consumer.at(j);
        if (diff > 0) {
            basis.emplace_back(cell(c.at(i).at(j), coordinate(i, j)));
            storage.at(i) -= x.at(i).at(j++);
        } else if (diff < 0) {
            basis.emplace_back(cell(c.at(i).at(j), coordinate(i, j)));
            consumer.at(j) -= x.at(i++).at(j);
        } else {
            if (j == n - 1 && n + m - 1 != basis.size()) {
                while (i < m) {
                    basis.emplace_back(cell(c.at(i).at(j), coordinate(i, j)));
                    i++;
                }
            } else if (i == m - 1 && n + m - 1 != basis.size()) {
                while (j < n) {
                    basis.emplace_back(cell(c.at(i).at(j), coordinate(i, j)));
                    j++;
                }
            } else {
                basis.emplace_back(cell(c.at(i).at(j), coordinate(i, j)));
                if (j + 1 != n)
                    basis.emplace_back(cell(c.at(i).at(j + 1), coordinate(i, j + 1)));
                else if (i + 1 != m)
                    basis.emplace_back(cell(c.at(i + 1).at(j), coordinate(i + 1, j)));
                i++;
                j++;
            }
        }
    }
    auto v = vector(n);
    auto u = vector(m);
    cell minCell;
    std::vector<cell> cycleCells;
    while (true) {
        potentialsCalculation(c, basis, v, u, n);
        minCell = findNonBasisMinimum(c, basis, v, u);
        if (minCell.first == 0)
            return;
        basis.emplace_back(minCell);
        cycleCells = deletingMethod(basis, n, m);
        parseCycle(cycleCells, minCell, x, basis);
    }
}

int main(int argc, const char * argv[]) {
    std::ifstream f("input.txt");
    if (!f.is_open()) {
        return -9;
    }
    int m, n;
    f >> m >> n;
    matrix c(m, vector(n));
    matrix x(m, vector(n));
    vector a(m);
    vector b(n);
    inputFor(f, a);
    inputFor(f, b);
    inputFor(f, c);
    matrixTransportTask(x, c, a, b);
    int result = 0;
    for (unsigned long i = 0; i < m; ++i) {
        for (unsigned long j = 0; j < n; ++j) {
            result += c.at(i).at(j) * x.at(i).at(j);
        }
    }
    std::ofstream o("output.txt");
    o << result;
    return 0;
}
