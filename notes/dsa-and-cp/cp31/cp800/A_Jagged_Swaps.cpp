#include<bits/stdc++.h>
using namespace std;

bool f(vector<int> &arr, int n)
{
    bool swapped = false;
    do
    {
        swapped = false;
        for (int i = 1; i <= n - 2; i++)
        {
            if (arr[i - 1] < arr[i] && arr[i] > arr[i + 1])
            {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    } while (swapped);

    for (int i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
            return false;
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
            int n;
            cin >> n;
            vector<int> arr(n);
            for (int i = 0; i < n; i++)
                cin >> arr[i];

            bool sort = f(arr, n);
            if (sort)
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
        }

        return 0;
    }