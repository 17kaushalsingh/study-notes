#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <array>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>
#include <memory>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <chrono>
#include <complex>
#include <stdexcept>
#include <exception>
#include <type_traits>
#include <bitset>
#include <tuple>
#include <iterator>
#include <cassert>
#include <queue>
#define ll long long
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