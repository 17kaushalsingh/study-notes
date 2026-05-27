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
    int n, k;
    cin >> n >> k;

    priority_queue<pair<int, int>> pq;
    for (int i=0; i<n; i++) {
        int num;
        cin >> num;

        num = num % k;
        if (num == 0) num = k;

        pq.push({num, -i});
    }

    vector<int> ans;

    while(!pq.empty()) {
        pair<int, int> p = pq.top();
        pq.pop();

        int h = p.first - k;
        if (h > 0) {
            pq.push({h, p.second});
        } else {
            ans.push_back(-p.second);
        }
    }

    for (int &num : ans) {
        cout << num+1 << " "; // 1-based indexing
    }
    cout << "\n";
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