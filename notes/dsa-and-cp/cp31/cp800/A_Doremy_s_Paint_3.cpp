#include<bits/stdc++.h>
using namespace std;

bool f(vector<int> &arr, int n)
{
    map<int, int> mp;
    for (int i=0; i<n; i++)
    {
        mp[arr[i]]++;
        if (mp.size() > 2) return false;
    }

    if (mp.size() == 2)
    {
        vector<int> cnt;
        for (auto &it : mp) cnt.push_back(it.second);

        return (abs(cnt[0]-cnt[1]) <= 1);
    }

    return true;
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
        vector<int> arr(n);
        for (int i=0; i<n; i++) cin >> arr[i];

        bool ans = f(arr, n);
        if (ans) cout << "Yes" << endl;
        else cout << "No" << endl;
    }

    return 0;
}