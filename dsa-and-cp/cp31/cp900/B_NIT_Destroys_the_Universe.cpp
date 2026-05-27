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

void solve() {
    int n; cin >> n;
    vector<int> a = takeInput(n);

    int cnt = 0;
    for (int &x : a) if (x == 0) cnt++;

    if (cnt == n) {
        cout << 0 << endl;
        return;
    } else if (cnt == 0) {
        cout << 1 << endl;
        return;
    } else {
        int k = cnt;
        for (int i=0; i<cnt; i++) {
            if (a[i] == 0) k--;
            else break;
        }
        for (int i=n-1; i>=n-cnt; i--) {
            if (a[i] == 0) k--;
            else break;
        }
        if (k == 0) {
            cout << 1 << endl;
            return;
        }
        cout << 2 << endl;
    }
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