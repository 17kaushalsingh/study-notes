#include <bits/stdc++.h>
typedef long long ll;
using namespace std;

void f(ll k, ll x)
{
    ll all = (1LL << (k+1));
    ll a = x;
    ll b = all - x;

    vector<int> path;
    // chocola to vanila 1
    // vanila to chocola 2
    while(a != b)
    {
        if (a < b) // in the prev step chocola must have given to vanila
        {
            b = b-a;;
            a = 2*a;
            path.push_back(1);
        }

        else
        {
            a = a - b;
            b = 2 * b;
            path.push_back(2);
        }
    }

    reverse(path.begin(), path.end());

    cout << path.size() << endl;
    for (int &x : path) cout << x << " ";
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
        ll k, x;
        cin >> k >> x;

        f(k, x);
    }

    return 0;
}