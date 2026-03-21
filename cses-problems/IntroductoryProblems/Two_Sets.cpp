#include<iostream>

int main() {
    int n = {}; std::cin >> n;

    bool possible = {false};

    possible = (((n*(n+1)/2) % 2 )== 0) ? (true) : false;

    if (!possible) {
        std::cout << "NO";
    } else {
        std::cout << "YES";
    }
}