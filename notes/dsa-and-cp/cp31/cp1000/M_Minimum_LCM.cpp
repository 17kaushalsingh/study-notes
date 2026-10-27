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
    cin >> n;

    int a = 1, b = n - 1;
    for (int i=2; i*i<=n; i++) {
        if (n % i == 0) {
            a = n/i;
            b = n - a;
            break;
        }
    }

    cout << a << " " << b << "\n";
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
