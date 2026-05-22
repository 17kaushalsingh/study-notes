#include <bits/stdc++.h>
using namespace std;

// Speed and Type Definitions
#define int long long
#define endl "\n"
#define pb push_back
#define all(v) v.begin(), v.end()

const int mod = 1e9 + 7;
const int inf = 1e18;

void fast_io() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // cout.tie(NULL); // Technically redundant but harmless
}

int exp(int x, int y) {
    // comutes x ^ y

    int res = 1;
    while(y > 0) {
        if (y & 1) {
            res = res * x % mod;
            y--;
        } else {
            x = x * x % mod;
            y = y >> 1;
        }
    }

    return res;
}

void solve(int t) {
    int n; cin >> n;
    int ans = exp(2, n) % mod;
    cout << ans << endl;
}

#undef int
int main() {
    fast_io();
    
    // For floating point precision problems
    // cout << fixed << setprecision(10);

    int t = 1;
    // cin >> t; // Remove if only one test case

    for (int i = 1; i <= t; i++) {
        solve(i);
    }

    return 0;
}