#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<ll> takeInput(ll n) {
    vector<ll> arr(n);
    for (ll i=0; i<n; i++) {
        cin >> arr[i];
    }
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

void solve()
{
    ll n; cin >> n;
    vector<ll> a = takeInput(n);

    ll xr = 0;
    for (ll i=0; i<n; i++) xr = xr ^ a[i];

    if (!(n&1))
    {
        if (xr == 0) cout << 1 << endl;
        else cout << -1 << endl;
        return;
    } 

    else if (xr >= 0 && xr < pow(2, 8)) cout << xr << endl;
    else cout << -1 << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}