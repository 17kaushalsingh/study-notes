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
        long long a, b;
        cin >> a >> b;

        long long maxSum = -1;
        long long sum = a+b;
        if (sum%2 == 0) maxSum = sum; 

        

        cout << maxSum << endl;
    }

    return 0;
}