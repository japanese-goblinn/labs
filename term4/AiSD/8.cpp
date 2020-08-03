#include <iostream>

bool equals(std::string s) {
    u_long n = s.length();
    for (int i = 1; i < n; i++)
        if (s[i] != s[0]) {
            return false;
        }
    return true;
}

int main(int argc, const char * argv[]) {
    std::string pol;
    std::cin >> pol;
    if (equals(pol)) {
        std::cout << -1;
    } else {
        int end = (int)pol.length();
        int center = end / 2;
        int count = 0;
        int j = end - 1;
        for(int i = 0;  i < center; i++) {
            if (pol[i] != pol[j]) {
                std::cout << end;
                count++;
                break;
            }
            j--;
        }
        if (!count) {
            std::cout << (int)pol.length() - 1;
        }
    }
