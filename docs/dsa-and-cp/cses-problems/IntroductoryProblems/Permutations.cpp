#include<iostream>

int main() {
    long n;
    std::cin >> n;

    if (n == 1) {
        std::cout << 1;
    } else if (n <= 3) {
        std::cout << "NO SOLUTION";
    }  else if (n == 4) {
        std::cout << "2 4 1 3";
    } else {
        long start = 1;
        
        while(start <= n) {
            std::cout << start << " ";
            start += 2;
        }

        start = 2;
        while(start <= n) {
            std::cout << start << " ";
            start += 2;
        }
    }

    return EXIT_SUCCESS;
}