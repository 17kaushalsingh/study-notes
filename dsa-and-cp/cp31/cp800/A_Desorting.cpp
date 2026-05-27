#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    int n; cin >> n;
    vector<int> a(n);
    for (int i=0; i<n; i++) cin >> a[i];

    int minDiff = INT_MAX;
    for (int i=1; i<n; i++)
    {
        int diff = a[i] - a[i-1];
        minDiff = min(minDiff, diff);
    }

    if (minDiff < 0) 
    {
        cout << 0 << endl;
        return;
    }

    int ops = minDiff / 2 + 1;
    cout << ops << endl;
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