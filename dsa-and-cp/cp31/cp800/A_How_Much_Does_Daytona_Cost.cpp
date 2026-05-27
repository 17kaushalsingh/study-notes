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
using namespace std;

bool f(vector<int> &arr, int n, int k)
{
    for (int num : arr) if (num == k) return true;
    return false;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        int n, k; cin >> n >> k;
        vector<int> arr(n);
        for (int i=0; i<n; i++) cin >> arr[i];

        bool ans = f(arr, n, k);
        if (ans) cout << "YES" << endl;
        else cout << "NO" << endl;
    }

    return 0;
}