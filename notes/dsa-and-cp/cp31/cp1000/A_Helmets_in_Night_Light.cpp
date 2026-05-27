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

static bool comp(const pair<int, int> &p1, const pair<int, int> &p2) {
    int a1 = p1.first, b1 = p1.second;
    int a2 = p2.first, b2 = p2.second;

    if (b1 == b2) {
        return (a1 > a2);
    } else {
        return (b1 < b2);
    }
}

void solve() {
    int n, p;
    cin >> n >> p;

    vector<pair<int, int>> residents(n);
    for (int i=0; i<n; i++) {
        int &a = residents[i].first; // persons they can inform
        cin >> a;
    }

    for (int i=0; i<n; i++) {
        int &b = residents[i].second; // cost to inform each person
        cin >> b;
    }

    int totalCost = p;
    int personsInformed = 1;

    sort(all(residents), comp);

    for (int i=0; i<n; i++) {
        int a = residents[i].first;
        int b = residents[i].second;

        if (b >= p || personsInformed >= n) {
            break;
        }

        totalCost += b * min(a, n - personsInformed);
        personsInformed += min(a, n - personsInformed);
    }

    totalCost += p * (n - personsInformed);

    cout << totalCost << "\n";
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
