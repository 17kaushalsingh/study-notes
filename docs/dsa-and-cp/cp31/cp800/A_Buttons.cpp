#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve()
{
    int a, b, c;
    cin >> a >> b >> c;

    if (a < b)
    {
        cout << "Second" << endl;
        return;
    }

    else if (b < a)
    {
        cout << "First" << endl;
        return;
    }

    else{
        int turn = (c & 1) ? 2 : 1;
        if (turn == 1) cout << "Second" << endl;
        else cout << "First" << endl;
    }
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