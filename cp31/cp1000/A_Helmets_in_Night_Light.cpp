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

static bool comp(const pair<int, int> &p1, const pair<int, int> &p2) {
    if (p1.second != p2.second) {
        return p1.second < p2.second;
    } else {
        return p1.first > p2.first;
    }
}

void solve() {
    int n; cin >> n;
    int p; cin >> p;
    vector<int> a = takeInput(n);
    vector<int> b = takeInput(n);

    vector<pair<int, int>> pairs(n);
    for (int i=0; i<n; i++) {
        pair<int, int> pr = make_pair(a[i], b[i]);
        pairs[i] = pr;
    }

    sort(all(pairs));
    
    int cost = 0;

    cout << cost << endl;

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