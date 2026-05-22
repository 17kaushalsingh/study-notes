// ===== Core I/O & Utilities =====
#include <iostream>
#include <iomanip>
#include <ios>
#include <limits>
#include <cassert>

// ===== Containers =====
#include <vector>
#include <array>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

// ===== Algorithms & Iterators =====
#include <algorithm>
#include <numeric>
#include <iterator>

// ===== Strings & Parsing =====
#include <string>
#include <cstring>
#include <sstream>

// ===== Math & Bit Operations =====
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <climits>
#include <bitset>

// ===== Time & Random =====
#include <chrono>
#include <random>

using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int mod = (int)(1e9+7);

vector<int> takeInput(int n) {
    vector<int> arr(n);
    for (int &x : arr) cin >> x;
    return arr;
}

void printArray(vector<int> &arr) {
    for (int &x : arr) cout << x << " ";
    cout << "\n";
}

void solve() {
    int s, k, m; cin >> s >> k >> m;
    
}

#undef int
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
