#include <bits/stdc++.h>
using namespace std;

long long f(vector<long long> &arr, long long n, long long m)
{

    sort(arr.begin(), arr.end(), greater<long long>());

    long long mini = min(m, n);

    long long ans = 0;
    for (long long i=0; i<mini; i++)
    {
        ans += arr[i] * (m - i);
    }

    return ans;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        long long n, m;
        cin >> n >> m;
        vector<long long> a(n);
        for (long long i=0; i<n; i++) cin >> a[i];

        cout << f(a, n, m) << endl;
    }

    return 0;
}