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
        int n; cin >> n;

        bool vanya = (n % 3 != 0);
        (vanya) ? (cout << "First") : (cout << "Second");
        cout << endl;
    }

    return 0;
}