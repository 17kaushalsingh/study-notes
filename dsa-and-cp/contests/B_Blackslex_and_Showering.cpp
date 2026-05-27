#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int mod = (int)(1e9+7);

vector<int> takeInput(int n) {
    vector<int> arr(n);
    for (int &x : arr) cin >> x;
    return arr;
}

void printArray(vector<int> &arr) {
    for (int &x : arr) cout << x << " ";
    cout << "\n";
}

void solve() {
    int n;
    cin >> n;
    vector<int> a = takeInput(n);

    int maxIndex = 0;
    int maxi = a[0];
    for (int i=0; i<n; i++) {
        if (a[i] > maxi) {
            maxi = a[i];
            maxIndex = i;
        }
    }

    vector<int> a1, a2;
    for (int i=0; i<maxIndex; i++) {
        a1.push_back(a[i]);
    }

    for (int i=maxIndex+1; i<n; i++) {
        a2.push_back(a[i]);
    }
    // cout << "Hello" << endl;
    // cout << a1.size() << " " << a2.size() << endl;

    int sum = 0;
    if (!a1.empty()) {
        for (int i=0; i<a1.size()-1; i++) {
            sum += abs(a1[i] - a1[i+1]);
        }
    }

    if (!a2.empty()) {
        for (int i=0; i<a2.size()-1; i++) {
            sum += abs(a2[i] - a2[i+1]);
        }
    }

    if (a1.size() != 0 && a2.size() != 0) {
        sum += abs(a1.back() - a2[0]);
    }

    cout << sum << endl;
}

#undef int
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
