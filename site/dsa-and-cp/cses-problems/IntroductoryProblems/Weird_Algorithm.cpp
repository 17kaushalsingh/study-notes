#include<iostream>

int main() {
    long n; std::cin >> n;

    while(n != 1) {
        std::cout << n << " ";
        if (n&1) n = 3 * n + 1;
        else n = n >> 1;
    }
    std::cout << n;

    return EXIT_SUCCESS;
}