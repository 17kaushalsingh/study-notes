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
    int a, b, c;
    cin >> a >> b >> c;

    if (((2*b-c) % a == 0) && ((2*b-c) / a) > 0) {
        cout << "YES" << endl;
        return;
    } else if (((c+a)%(2*b) == 0) && ((c+a)/(2*b)) > 0) {
        cout << "YES" << endl;
        return;
    } else if (((2*b-a)%c == 0) && ((2*b-a)/c) > 0) {
        cout << "YES" << endl;
        return;
    } else {
        cout << "NO" << endl;
        return;   
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