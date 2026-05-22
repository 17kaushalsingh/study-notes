#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    int n; cin >> n;
    vector<int> a(n);
    int sum = 0;
    int neg = 0;
    for (int i=0; i<n; i++)
    {
        int temp; cin >> temp;
        sum += temp;
        if (temp < 0) neg++;
    }

    if (sum >= 0) {
        if (neg%2 == 0) cout << 0 << endl;
        else cout << 1 << endl;
    } else { // sum < 0 
        int need = (-sum + 1) / 2;
        neg -= need;
        if (neg%2 == 1) need++;
        cout << need << endl;
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