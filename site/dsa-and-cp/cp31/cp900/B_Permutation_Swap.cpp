#include <bits/stdc++.h>
#include <numeric>
#define ll long long
#define all(x) (x).begin(), (x).end()
using namespace std;

vector<ll> takeInput(ll n) {
    vector<ll> arr(n);
    for (ll i=0; i<n; i++) cin >> arr[i];
    return arr;
}

void printArray(vector<ll> &arr) {
    for (ll &x : arr) cout << x << " ";
    cout << "\n";
}

ll getMin(vector<ll> &arr) {
    ll mini = LLONG_MAX;
    for (ll &x : arr) mini = min(mini, x);
    return mini;
}

ll getMax(vector<ll> &arr) {
    ll maxi = LLONG_MIN;
    for (ll &x : arr) maxi = max(maxi, x);
    return maxi;
}

ll getSum(vector<ll> &arr) {
    ll sum = 0;
    for (ll &x : arr) sum += x;
    return sum;
}

void solve() {
    ll n; cin >> n;

    vector<ll> p(n);
    vector<ll> diff;
    for (ll i=0; i<n; i++) {
        cin >> p[i];

        ll temp = abs(i+1 - p[i]);
        if (temp != 0) diff.push_back(temp);
    }

    if (diff.size() == 0) {
        cout << 1 << endl;
        return;
    }

    ll hcf = diff[0];
    for (ll &x : diff) hcf = gcd(hcf, x);

    cout << hcf << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) solve();
    return 0;
}