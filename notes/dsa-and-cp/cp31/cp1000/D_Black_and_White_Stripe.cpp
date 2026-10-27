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
    int n, k; cin >> n >> k;
    string stripe; cin >> stripe;

    int l = 0; int r = k-1;
    int cnt = 0; // count of blacks in current seq
    for (int i=l; i<=r; i++) {
        if (stripe[i] == 'B') {
            cnt++;
        }
    }

    int minChange = INT_MAX;
    while(r < n) {
        int req = k - cnt;
        if (req <= 0) {
            minChange = 0;
            break;
        }

        if (req < minChange) {
            minChange = req;
        }
        
        if (stripe[l] == 'B') {
            cnt--;
        }
        
        if (r+1 < n && stripe[r+1] == 'B') {
            cnt++;
        }

        l++; r++;
    }

    cout << minChange << endl;
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