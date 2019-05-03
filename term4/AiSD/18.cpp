#include <iostream>


int main(int argc, const char * argv[]) {
    int year;
    std::cin >> year;
    if ((!(year % 4) && year % 100) || !(year % 400)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
