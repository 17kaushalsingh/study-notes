#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    ll n; cin >> n;
    vector<ll> a(n);
    for (ll i=0; i<n; i++) cin >> a[i];

    vector<ll> odds;
    vector<ll> evens;
    for (ll i=0; i<n; i++)
    {
        if (a[i]&1) odds.push_back(a[i]);
        else evens.push_back(a[i]);
    }

    if (odds.size() == 0) {
        cout << 0 << endl;
        return;
    }

    ll ans = 0;
    for (ll &x : evens) ans += x;

    ll m = odds.size();
    sort(odds.begin(), odds.end(), greater<int>());

    for (ll i=0; i<(m+1)/2; i++) ans += odds[i];
    
    cout << ans << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll t;
    cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}