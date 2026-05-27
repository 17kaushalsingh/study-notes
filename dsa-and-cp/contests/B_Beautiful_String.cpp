#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        long long a, b;
        cin >> a >> b;

        if (a == b) {
            cout << "0\n";
            continue;
        }

        // Compute M = (1 << (bit_length of a)) - 1
        int bits = 0;
        long long temp = a;
        while (temp > 0) {
            bits++;
            temp >>= 1;
        }
        long long M = (1LL << bits) - 1;

        if (b > M) {
            cout << "-1\n";
            continue;
        }

        long long x = a ^ b;
        if (x <= a) {
            cout << "1\n";
            cout << x << "\n";
        } else {
            long long x1 = a ^ M;
            long long x2 = M ^ b;
            cout << "2\n";
            cout << x1 << " " << x2 << "\n";
        }
    }

    return 0;
}