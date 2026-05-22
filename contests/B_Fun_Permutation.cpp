#include<bits/stdc++.h>
using namespace std;

vector<int> f(vector<int> &p, int n)
{
    vector<int> q(n);
    for (int i=0; i<n; i++) q[i] = n + 1 - p[i];

    return q;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;

        vector<int> arr(n);
        for (int i=0; i<n; i++) cin >> arr[i];

        vector<int> ans = f(arr, n);
        for (int &x : ans) cout << x << " ";
        cout << endl;

    }

    return 0;
}