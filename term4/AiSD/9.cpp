#include <iostream>
#include <vector>

int main(int argc, const char * argv[]) {
    std::string string;
    std::cin >> string;
    std::vector<bool> bools((int)string.length() + 1, false);
    int n;
    std::cin >> n;
    int start = 0, end = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> start >> end;
        if (end < start) {
            int tmp = end;
            end = start;
            start = tmp;
        }
        bools[start - 1] = !bools[start - 1];
        bools[end] = !bools[end];
    }
    bool to_invert = false;
    for (int i = 0; i < (int)string.length(); i++) {
        if (bools[i]) {
            to_invert = !to_invert;
        }
        if (to_invert) {
            if (islower(string[i])) {
                string[i] = string[i] - 32;
            } else {
                string[i] = string[i] + 32;
            }
        }
    }
    std::cout << string;
