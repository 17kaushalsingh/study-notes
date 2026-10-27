#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include <climits>
#include <numeric>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <deque>
#include <utility>
#include <iomanip>
#include <functional>
using namespace std;

#define int long long
#define endl "\n"
#define pb push_back
#define all(v) v.begin(), v.end()

const int mod = 1e9 + 7;
const int inf = 1e18;

void solve() {
    int n;
    char ch;

    cin >> n >> ch;

    string s;
    cin >> s;

    if (ch == 'g') {
        cout << 0 << "\n";
        return;
    }

    vector<int> greens;
    for (int i=0; i<n; i++) {
        if (s[i] == 'g') {
            greens.pb(i);
        }
    }

    int maxi = INT_MIN;
    for (int i=0; i<n; i++) {
        if (s[i] == ch) {
            auto it = lower_bound(all(greens), i);
            if (it == greens.end()) {
                maxi = max(maxi, n - i + greens[0]);
            } else {
                maxi = max(maxi, (*it) - i);
            }
        }
    }

    cout << maxi << "\n";
}

#undef int
int main() {
    // For floating point precision problems
    // cout << fixed << setprecision(10);

    int t = 1;
    cin >> t;

    for (int i = 1; i <= t; i++) {
        solve();
    }

    return 0;
}
