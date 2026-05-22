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

int getMin(vector<int> &arr) {
    int mini = LLONG_MAX;
    for (int &x : arr) mini = min(mini, x);
    return mini;
}

int getMax(vector<int> &arr) {
    int maxi = LLONG_MIN;
    for (int &x : arr) maxi = max(maxi, x);
    return maxi;
}

int getSum(vector<int> &arr) {
    int sum = 0;
    for (int &x : arr) sum += x;
    return sum;
}

void solve() {
    int n; cin >> n;
    vector<int> a = takeInput(n);
    int maxi = getMax(a);
    int mini = getMin(a);
    int sum = getSum(a);

    cout << "mini: " << mini << " maxi: " << maxi << " sum: " << sum << endl;
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