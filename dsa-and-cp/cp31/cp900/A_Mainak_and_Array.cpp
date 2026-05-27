#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()

vector<int> takeInput(int n) {
    vector<int> arr(n, 0);
    for (int i=0; i<n; i++) cin >> arr[i];
    return arr;
}

void printArray(vector<int> &arr) {
    for (int &x : arr) cout << x << " ";
    cout << "\n";
}

void solve() {
    int n; cin >> n;
    vector<int> a = takeInput(n);

    if (n == 1) {
        cout << 0 << endl;
        return;
    }

    // rotate entire array
    int ans = a[n-1] - a[0];
    for (int i=0; i<n-1; i++) {
        ans = max(ans, a[i] - a[i+1]);
    }

    // do not touch a[0]
    int m = a[1];
    for (int i=1; i<n; i++) m = max(m, a[i]);
    ans = max(ans, m - a[0]);

    // do not touch a[n-1]
    m = a[0];
    for (int i=0; i<n-1; i++) m = min(m, a[i]);
    ans = max(ans, a[n-1] - m);

    cout << ans << endl;
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