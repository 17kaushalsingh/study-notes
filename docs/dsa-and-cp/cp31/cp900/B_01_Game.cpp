#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()

vector<int> takeInput(int n) {
    vector<int> arr(n);
    for (int i=0; i<n; i++) cin >> arr[i];
    return arr;
}

void printArray(vector<int> &arr) {
    for (int &x : arr) cout << x << " ";
    cout << "\n";
}

bool f(string &s, int n) {
    int cnt = 0;
    int c0 = 0, c1 = 0;
    for (int i=0; i<n; i++) {
        if (s[i] == '0') c0++;
        else c1++;
    }
    cnt = min(c0, c1);

    if (cnt&1) return true;
    return false;
}

void solve() {
    string s; cin >> s;
    int n = s.size();

    bool ans = f(s, n);
    if (ans) cout << "DA";
    else cout << "NET";
    cout << endl;
}

#undef int
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}