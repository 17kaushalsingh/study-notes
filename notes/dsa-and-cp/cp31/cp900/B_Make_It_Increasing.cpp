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

    if (n == 1) {
        cout << 0 << endl;
        return;
    }

    int cnt = 0;
    for (int i=n-2; i>=0; i--) {
        if (a[i+1] == 0) {
            cout << -1 << endl;
            return;
        }

        while(a[i] >= a[i+1]) {
            cnt++;
            a[i] = a[i] >> 1;
        }
    }

    cout << cnt << endl;
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