#include <iostream>
#include <vector>

long long result = 0;

void merge(std::vector<int> &mass, std::vector<int> &temp, int left, int right, int middle) {
    int left_part_count = left;
    int right_part_count = middle + 1;
    int index_count = left;
    while (left_part_count <= middle && right_part_count <= right) {
        if (mass[left_part_count] > mass[right_part_count]) {
            temp[index_count] = mass[right_part_count];
            index_count++;
            right_part_count++;
            result += (middle + 1 - left_part_count);
        } else {
            temp[index_count] = mass[left_part_count];
            index_count++;
            left_part_count++;
        }
    }
    while (left_part_count <= middle) {
        temp[index_count] = mass[left_part_count];
        index_count++;
        left_part_count++;
    }
    while (right_part_count <= right) {
        temp[index_count] = mass[right_part_count];
        index_count++;
        right_part_count++;
    }
    for (int i = left; i < index_count; i++) {
        mass[i] = temp[i];
    }
    return;
}

void m_sort(std::vector<int> &mass, int left, int right, std::vector<int> &temp) {
    if (left < right) {
        int middle = (left + right) / 2;
        m_sort(mass, left, middle, temp);
        m_sort(mass, middle + 1, right, temp);
        merge(mass, temp, left, right, middle);
    }
    return;
}

int main(int argc, const char * argv[]) {
    int n;
    std::cin >> n;
    std::vector<int> perms(n);
    std::vector<int> temp((int)1e6 + 1);
    for (int i = 0; i < perms.size(); i++) {
        std::cin >> perms[i];
    }
    m_sort(perms, 0, (int)perms.size() - 1, temp);
    std::cout << result;
