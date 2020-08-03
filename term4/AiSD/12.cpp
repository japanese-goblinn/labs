#include <iostream>


int main(int argc, const char * argv[]) {
    long long num;
    std::cin >> num;
    int win = num % 10;
    if (win) {
       std::cout << win;
    } else {
        std::cout << "NO";
    }
    return 0;
