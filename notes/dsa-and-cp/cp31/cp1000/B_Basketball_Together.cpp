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
    int n, d;
    cin >> n >> d;

    vector<int> p(n);
    for (int i=0; i<n; i++) {
        cin >> p[i];
    }

    sort(all(p));

    int teams = 0;
    int playersRemaining = n;
    int j = -1;

    for (int i=n-1; i>=0; i--) {
        if (j > i) {
            break;
        }

        int req = d / p[i] + 1;
        if (playersRemaining >= req) {
            teams++;
            playersRemaining -= req;
            j += req - 1;
        } else {
            break;
        }
    }

    cout << teams << "\n";
}

#undef int
int main() {
    // For floating point precision problems
    // cout << fixed << setprecision(10);

    int t = 1;
    // cin >> t;

    for (int i = 1; i <= t; i++) {
        solve();
    }

    return 0;
}
