#include<iostream>

long safeWays(long n) {
    long ans = (n*n * (n*n - 1) / 2) - (4 * (n-1)*(n-2));
    if (ans < 0) ans = 0;
    return ans;
}

int main() {
    long n; std::cin >> n;

    for (long k=1; k<=n; k++) {
        std::cout << safeWays(k) << "\n";
    }

    return EXIT_SUCCESS;
}