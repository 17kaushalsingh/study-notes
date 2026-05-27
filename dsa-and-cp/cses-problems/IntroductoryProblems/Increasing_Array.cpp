#include<iostream>
#include<vector>

int main() {
    long n; std::cin >> n;

    std::vector<long> nums(n);
    for (int i=0; i<n; i++) {
        std::cin >> nums[i];
    }

    long ops = 0;
    for (int i=1; i<n; i++) {
        if (nums[i] < nums[i-1]) {
            ops += nums[i-1] - nums[i];
            nums[i] = nums[i-1];
        }
    }

    std::cout << ops;
}