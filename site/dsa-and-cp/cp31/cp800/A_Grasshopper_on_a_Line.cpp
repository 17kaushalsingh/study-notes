#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    int x, k;
    cin >> x >> k;

    vector<int> path;
    for (int step=x; step>=1; step--)
    {
        if (x == 0) break;

        if (step % k != 0 && x/step >= 1) {
            int ops = x / step;
            for (int i=0; i<ops; i++) path.push_back(step);
            x -= ops * step;
        }
    }

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
        solve();
    }

    return 0;
}