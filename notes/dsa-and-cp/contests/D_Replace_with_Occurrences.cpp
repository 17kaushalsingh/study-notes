#include<bits/stdc++.h>
using namespace std;

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
        map<int, bool> freq;
        for (int i = 0; i < n; i++)
        {
            int temp;
            cin >> temp;
            freq[temp] = true;
        }

        int sum = 0;
        for (auto &it : freq) sum += it.first;

        if (sum == n)
        {
            int x = 1;
            for (auto &it : freq)
            {
                int cnt = it.first;
                while (cnt--)
                    cout << x << " ";
                x++;
            }
            cout << endl;
        }

        else
            cout << -1 << endl;
    }

    return 0;
}