#include <bits/stdc++.h>
using namespace std;

int f(int k, int x)
{
    while(k--)
    {
        if ((x-1)%3 == 0 && ((x-1)/3 % 2) == 1) x = (x-1)/3;
        else x = 2*x;
    }

    return x;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        int k, x;
        cin >> k >> x;

        int ans = f(k, x);
        cout << ans << endl;
    }

    return 0;
}