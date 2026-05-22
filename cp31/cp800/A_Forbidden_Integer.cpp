#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int getMinFactor(int n)
{
    for (int i=2; i<=n; i++) {
        if (n % i == 0) return i;
    }

    return INT_MAX;
}

void solve()
{
    int n, k , x;
    cin >> n >> k >> x;
    
    if (x != 1) {
        cout << "YES" << endl << n << endl;
        for (int i=0; i<n; i++) cout << 1 << " ";
        cout << endl;
    } else if (k == 1) { // k==1 and x==1
        cout << "NO" << endl;
    } else { // k >= 2
        if (n%2 == 0 ) {
            cout << "YES" << endl;
            cout << n/2 << endl;
            for (int i=0; i<n/2; i++) cout << 2 << " ";
            cout << endl;
        }

        else {
            if (k == 2) cout << "NO" << endl;
            else {
                // n is odd, and k is >= 3
                cout << "YES" << endl;
                cout << 1 + (n-3)/2 << endl;
                cout << 3 << " ";
                for (int i=0; i<(n-3)/2; i++) cout << 2 << " ";
                cout << endl;

            }
        }
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