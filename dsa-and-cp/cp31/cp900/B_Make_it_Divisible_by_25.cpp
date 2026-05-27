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

int f(string n, string val) {
    int tarIdx = 1;
    int ops = 0;
    for (int i=n.size()-1; i>=0; i--) {
        if (n[i] == val[tarIdx]) {
            tarIdx--;
            if (tarIdx < 0) return ops;
        } else {
            ops++;
        }
    }

    if (tarIdx >= 0) ops = INT_MAX;
    return ops;
}

void solve() {
    string n;
    cin >> n;

    vector<string> possibleValues = {"00", "25", "50", "75"};
    int minOps = INT_MAX;
    for (string val : possibleValues) {
        minOps = min(minOps, f(n, val));
    }

    cout << minOps << endl;
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