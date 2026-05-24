#include<bits/stdc++.h>
using namespace std;

int f(string &s, int n)
{
    int cnt = 0;
    for (int i=0; i<=n-3; i++)
    {
        if (s[i] == '.' && s[i+1] == '.' && s[i+2] == '.') return 2;
        if (s[i] == '.') cnt++;
    }
    if (s[n-2] == '.') cnt++;
    if (s[n-1] == '.') cnt++;

    return cnt;
}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        int n; cin >> n;
        string s; cin >> s;

        int ans = f(s, n);
        cout << ans << endl;
    }

    return 0;
}