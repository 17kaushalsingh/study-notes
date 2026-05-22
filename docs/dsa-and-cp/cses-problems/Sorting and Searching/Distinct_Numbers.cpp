#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int mod = (int)(1e9+7);

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
    vector<int> nums = takeInput(n);

    sort(all(nums));

    if (n <= 1) {
        cout << n << endl;
        return;
    } else {
        int cnt = 1;
        int last = nums[0];
        for (int i=1; i<n; i++) {
            if (nums[i] != last) {
                last = nums[i];
                cnt++;
            }
        }

        cout << cnt << endl;
        return;
    }
}

#undef int
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    // cin >> t;
    t = 1; // uncomment this line if only one test case
    while (t--) {
        solve();
    }
    return 0;
}

/*
*/