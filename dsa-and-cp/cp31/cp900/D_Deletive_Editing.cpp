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
    string s, target;
    cin >> s >> target;

    vector<int> freq(26, 0);
    for (char ch : target) freq[ch-'A']++;

    int n = s.size();
    for (int i=n-1; i>=0; i--) {
        char ch = s[i];
        if (freq[ch-'A'] > 0) {
            freq[ch-'A']--;
        } else {
            s[i] = '.';
        }
    }

    string rem = "";
    for (char ch : s) if (ch != '.') rem += ch;

    if (rem == target) {
        cout << "YES" << endl;
    } else cout << "NO" << endl;
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