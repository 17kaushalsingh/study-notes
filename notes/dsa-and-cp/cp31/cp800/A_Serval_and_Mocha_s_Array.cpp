#include <bits/stdc++.h>
#include <numeric>
using namespace std;
using ll = long long;

vector<ll> takeInput(ll n)
{
    vector<ll> arr(n);
    for (ll i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    return arr;
}

void printArray(vector<ll> &arr)
{
    for (ll &x : arr)
        cout << x << " ";
    cout << "\n";
}

ll getMin(vector<ll> &arr)
{
    ll mini = LLONG_MAX;
    for (ll &x : arr)
        mini = min(mini, x);
    return mini;
}

ll getMax(vector<ll> &arr)
{
    ll maxi = LLONG_MIN;
    for (ll &x : arr)
        maxi = max(maxi, x);
    return maxi;
}

ll getSum(vector<ll> &arr)
{
    ll sum = 0;
    for (ll &x : arr)
        sum += x;
    return sum;
}

ll gcd(ll a, ll b)
{
    while (b != 0)
    {
        ll temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool f(vector<ll> &arr, ll n)
{
    bool ok = false;
    for (ll i = 0; i < n; i++)
    {
        for (ll j = i + 1; j < n; j++)
        {
            if (gcd(arr[i], arr[j]) <= 2)
                return true;
        }
    }

    return false;
}

void solve()
{
    ll n;
    cin >> n;
    vector<ll> a = takeInput(n);

    bool ans = f(a, n);

    if (ans)
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
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