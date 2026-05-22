#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    int n;
    cin >> n;

    string s;
    cin >> s;

    string t = s;
    sort(t.begin(), t.end());

    int m = 0;
    for (int i=0; i<n; i++) if (s[i] != t[i]) m++;

    int ans = (m+1) / 2;
    cout << ans << endl;
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