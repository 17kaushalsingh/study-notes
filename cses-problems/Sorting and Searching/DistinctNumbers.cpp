#include<bits/stdc++.h>
using namespace std;
 
int main()
{
    int n;
    cin >> n;
 
    set<int> set;
    for (int i=0; i<n; i++)
    {
        int num;
        cin >> num;
        set.insert(num);
    }
 
    cout << set.size();
    return 0;
}