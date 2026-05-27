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
    vector<int> p = takeInput(n);

    for (int j=0; j<n; j++) {
        int i_ = -1;
        int k_ = -1;
        
        for (int i=j-1; i>=0; i--) {
            if (p[i] < p[j]) {
                i_ = i;
                break;
            }
        }

        for (int k=j+1; k<n; k++) {
            if (p[j] > p[k]) {
                k_ = k;
                break;
            }
        }

        if (i_ != -1 && k_ != -1) {
            cout << "YES" << endl;
            cout << i_+1 << " " << j+1 << " " << k_+1 << endl;
            return;
        }
    }

    cout << "NO" << endl;
    
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