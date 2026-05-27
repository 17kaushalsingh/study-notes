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
    
    int a = 0, b = 0;
    while(n > 0 && n % 2 == 0) {
        n /= 2;
        a++;
    }

    while(n > 0 && n % 3 == 0) {
        n /= 3;
        b++;
    }

    if (n > 1 || a > b) {
        cout << -1 << endl;
        return;
    }

    int ops = (b - a) + b;
    cout << ops << endl;
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