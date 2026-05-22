#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve()
{
    int n, m;
    cin >> n >> m;
    string x, s;
    cin >> x >> s;

    int found = false;
    int ops= 0;

    while(ops <= 10)
    {
        if (x.find(s) != string::npos)
        {
            cout << ops << endl;
            found = true;
            break;
        }

        x += x;
        ops++;
    }

    if (!found) cout << -1 << endl;
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