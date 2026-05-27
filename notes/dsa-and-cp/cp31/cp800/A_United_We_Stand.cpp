#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve()
{
    int n; cin >> n;
    vector<int> a(n);
    for (int i=0; i<n; i++) cin >> a[i];

    vector<int> b, c;

    sort(a.begin(), a.end());
    b.push_back(a[0]);

    for (int i=1; i<n; i++)
    {
        if (a[0] % a[i] != 0) c.push_back(a[i]);
        else b.push_back(a[i]);
    }

    if (b.size() == 0 || c.size() == 0)
    {
        cout << -1 << endl;
        return;
    }

    cout << b.size() << " " << c.size() << endl;
    for (int &x: b) cout << x << " ";
    cout << endl;
    for (int &x: c) cout << x << " ";
    cout << endl;
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