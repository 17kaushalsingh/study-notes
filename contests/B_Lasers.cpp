#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    int n, m, x, y;
    cin >> n >> m >> x >> y;
    vector<int> a(n);
    for (int i=0; i<n; i++) cin >> a[i];
    vector<int> b(m);
    for (int i=0; i<m; i++) cin >> b[i];

    cout << n + m << endl;
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