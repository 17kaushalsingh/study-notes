#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<int> f(vector<int> &arr, int n)
{
    if (arr.size() == 0) return {};

    vector<int> a;
    a.push_back(arr[0]);

    for (int i=1; i<n; i++)
    {
        if (arr[i] >= arr[i-1]) a.push_back(arr[i]);
        else{
            a.push_back(1);
            a.push_back(arr[i]);
        }
    }

    return a;
}

void solve()
{
    int n; cin >> n;
    vector<int> b(n);
    for (int i=0; i<n; i++) cin >> b[i];

    vector<int> a = f(b, n);

    cout << a.size() << endl;
    for (int &x : a) cout << x << " ";
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