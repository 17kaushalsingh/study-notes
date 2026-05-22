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

int getSum(int n) {
    if (n % 4 == 0) {
        return 0;
    } else if (n % 4 == 1) {
        return n;
    } else if (n % 4 == 2) {
        return -1;
    } else {
        return -(n+1);
    }
}

void solve() {
    int x; cin >> x;
    int n; cin >> n;

    int d = getSum(n);

    if (x % 2 == 0) {
        cout << x - d << endl;
    } else {
        cout << x + d << endl;
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