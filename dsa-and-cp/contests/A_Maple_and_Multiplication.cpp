#include <bits/stdc++.h>
using namespace std;

int f(int a, int b)
{
    if (a == b) return 0;
    int mini = min(a, b);
    int maxi = max(a, b);
    if (maxi % mini == 0) return 1;
    return 2;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        int a, b;
        cin >> a >> b;

        int ans = f(a, b);
        cout << ans << endl;
    }

    return 0;
}