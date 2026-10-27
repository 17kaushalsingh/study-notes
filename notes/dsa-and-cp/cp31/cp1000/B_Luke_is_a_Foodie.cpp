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

bool isValidIntersection(pair<int, int> &intersection) {
    int left = intersection.first;
    int right = intersection.second;

    return (left <= right);
}

void solve() {
    int n, x;
    cin >> n >> x;

    vector<int> a(n);
    for (int i=0; i<n; i++) {
        cin >> a[i];
    }

    int ans = 0;
    int left = a[0] - x;
    int right = a[0] + x;

    pair<int, int> intersection = make_pair(left, right);

    for (int i=0; i<n; i++) {
        int newLeft = max(intersection.first, a[i] - x);
        int newRight = min(intersection.second, a[i] + x);

        intersection = make_pair(newLeft, newRight);
        bool validIntersection = isValidIntersection(intersection);

        if (!validIntersection) {
            ans++;
            intersection = make_pair(a[i] - x, a[i] + x);
        }
    }

    cout << ans << endl;
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