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
    int n; cin >> n;
    int k; cin >> k;
    int b; cin >> b;
    int s; cin >> s;

    int remSum = s - k*b;
    int maxFillable = (n-1)*(k-1)+k-1;

    if (k*b > s || remSum > maxFillable) {
        cout << -1 << "\n";
        return;
    }

    vector<int> ans(n, 0);
    ans[0] = k*b;
    if (k-1 < remSum) {
        ans[0] += k-1;
        remSum -= k-1;
    } else {
        ans[0] += remSum;
        remSum -= remSum;
    }

    for (int i=1; i<n; i++) {
        if (remSum >= k-1) {
            ans[i] = k-1;
            remSum -= k-1;
        } else {
            ans[i] = remSum;
            break;
        }
    }

    for (int &x : ans) {
        cout << x << " ";
    }
    cout << "\n";
}

#undef int
int main() {
    // For floating point precision problems
    // cout << fixed << setprecision(10);

    int t = 1;
    cin >> t;

    while(t--) {
        solve();
    }

    return 0;
}