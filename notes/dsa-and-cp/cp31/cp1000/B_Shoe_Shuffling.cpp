#include<bits/stdc++.h>
using namespace std;

void solve() {
    int n; cin >> n;

    map<int, int> freq;
    for (int i=0; i<n; i++) {
        int num; cin >> num;
        freq[num]++;
    }

    vector<int> ans;

    for (auto it : freq) {
        int cnt = it.second;

        if (cnt == 1) {
            cout << -1 << endl;
            return;
        }

        int offset = ans.size() + 1;

        for (int i=1; i<=cnt-1; i++) {
            ans.push_back(offset + i);
        }
        ans.push_back(offset);
    }

    for (int num : ans) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }

    return 0;
}