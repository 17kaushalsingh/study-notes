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
    int a, b;
    cin >> a >> b;

    if (a == b) {
        cout << 0 << " " << 0 << endl;
        return;
    }

    if (b > a) swap(a, b);

    int g = a - b;
    int op1 = b % g;
    int op2 = g - (b % g);
    int minOps = min(op1, op2);

    cout << g << " " << minOps << endl;
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