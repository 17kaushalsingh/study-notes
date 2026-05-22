#include <bits/stdc++.h>
using namespace std;
using ll = long long;

bool f(vector<int> &arr, int n)
{
    int sum = accumulate(arr.begin(), arr.end(), 0);
    return (sum % 2 == 0);
}

void solve()
{
    int n; cin >> n;
    vector<int> a(n);
    for (int i=0; i<n; i++) cin >> a[i];

    bool ans = f(a, n);
    if (ans) cout << "YES" << endl;
    else cout << "NO" << endl;
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