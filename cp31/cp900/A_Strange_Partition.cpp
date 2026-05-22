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
    int n, x; cin >> n >> x;
    vector<int> a = takeInput(n);

    int maxi = 0;
    int sum = 0;
    for (int i=0; i<n; i++) {
        maxi += a[i] / x + ((a[i] % x != 0) ? 1 : 0);
        sum += a[i];
    }
    int mini = sum / x + ((sum % x != 0) ? 1 : 0);

    cout << mini << " " << maxi << endl;
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