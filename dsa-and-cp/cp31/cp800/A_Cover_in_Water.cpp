#include<bits/stdc++.h>
using namespace std;

int f(string &s, int n)
{
    for (int i=0; i<n-2; i++) {
        if (s[i] == '.' && s[i+1] == '.' && s[i+2] == '.') {
            return 2;
        }
    }

    int cnt = 0;
    for (char &ch : s) if (ch == '.') cnt++;

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