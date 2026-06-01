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

int getFreq(string &s, int l, int r) {
    unordered_set<char> st;
    for (int i=l; i<=r; i++) {
        st.insert(s[i]);
    }

    return st.size();
}

void solve() {
    int n;
    cin >> n;

    string s;
    cin >> s;

    int maxFreq = 1;
    int freq1 = 0;
    unordered_set<char> seen;
    for (int i=0; i<n; i++) {
        if (seen.find(s[i]) == seen.end()) {
            seen.insert(s[i]);
            freq1++;
            int freq2 = getFreq(s, i+1, n-1);

            maxFreq = max(maxFreq, freq1+freq2);
        }
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