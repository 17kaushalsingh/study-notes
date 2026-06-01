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

void f(vector<int> &nums, unordered_map<int, int> &map) {
    int cnt = 1;
    map[nums[0]] = 1;

    int n = nums.size();
    for (int i=1; i<n; i++) {
        if (nums[i] == nums[i-1]) {
            cnt++;
        } else {
            cnt = 1;
        }
        map[nums[i]] = max(map[nums[i]], cnt);
    }
}


void solve() {
    int n;
    cin >> n;

    vector<int> a(n);
    for (int i=0; i<n; i++) {
        cin >> a[i];
    };

    vector<int> b(n);
    for (int i=0; i<n; i++) {
        cin >> b[i];
    }

    unordered_map<int, int> freqA;
    f(a, freqA);
    unordered_map<int, int> freqB;
    f(b, freqB);

    int maxFreq = 1;
    for (int i=1; i<=2*n; i++) {
        int cnt1 = freqA[i];
        int cnt2 = freqB[i];

        maxFreq = max(maxFreq, cnt1 + cnt2);
    }

    cout << maxFreq << "\n";
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