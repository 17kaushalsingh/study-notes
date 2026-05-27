#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    int n, m;
    cin >> n >> m;
    vector<int> a(n+1), b(n+1);

    a[0] = 0;
    b[0] = 0;
    for (int i=1; i<=n; i++) cin >> a[i] >> b[i];

    ll ans = 0;

    // Process each requirement segment
    for (int i=1; i<=n; i++)
    {
        int d = a[i] - a[i-1];
        int needSwitch = (b[i] ^ b[i-1]); // 1 if sides differ, 0 if same

        // If parity matches, can use all d runs
        if ((d % 2) == needSwitch) ans += d;
        else ans += d - 1;
    }

    // After last requirement, free segment until m
    ans += (m - a[n]);

    cout << ans << "\n";
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
