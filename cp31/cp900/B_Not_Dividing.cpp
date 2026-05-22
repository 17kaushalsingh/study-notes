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

    int ops = 2*n;
    for (int i=0; i<n-1; i++) {
        if (ops <= 0) break;

        if (a[i] == 1) {
            a[i]++;
            ops--;
        }
    }

    for (int i=1; i<n; i++) {
        if (ops <= 0) break;

        if (a[i] % a[i-1] == 0) {
            a[i]++;
            ops--;
        }
    }

    printArray(a);
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