#include<iostream>

int main() {
    long xr = 0;
    
    long n; std::cin >> n;
    for (int i=1; i<=n-1; i++) {
        xr = xr ^ i;
        long temp; std::cin >> temp;
        xr = xr ^ temp;
    }

    xr = xr ^ n;

    std::cout << xr;

    return EXIT_SUCCESS;
}