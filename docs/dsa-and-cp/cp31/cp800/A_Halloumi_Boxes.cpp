#include<bits/stdc++.h>
using namespace std;

bool isSorted(vector<int>&arr, int n)
{
    for (int i=1; i<n; i++)
    {
        if (arr[i-1] > arr[i]) {
            return false;
        }
    }

    return true;
}

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        int k;
        cin >> k;

        vector<int> arr(n);
        for (int i=0; i<n; i++) cin >> arr[i];

        if (k >= 2 || isSorted(arr, n)) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}