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

void solve(int t) {
    int a, b; cin >> a >> b;
    
    bool possible = ((a + b) % 3) == 0;
    if (possible && (max(a, b) * 2 <= min(a, b))) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }
}

#undef int
int main() {
    fast_io();
    
    // For floating point precision problems
    // cout << fixed << setprecision(10);

    int t = 1;
    cin >> t; // Remove if only one test case

    for (int i = 1; i <= t; i++) {
        solve(i);
    }

    return 0;
}