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
    int n; cin >> n;

    int fives = 0;
    while(n >= 5) {
        fives += n / 5;
        n = n / 5;
    }

    cout << fives << endl;
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