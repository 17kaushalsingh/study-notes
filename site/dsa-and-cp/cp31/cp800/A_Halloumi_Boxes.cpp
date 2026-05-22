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

bool f(vector<int>&arr, int k)
{
    if (k >= 2) return true;

    int n = arr.size();
    for (int i=1; i<n; i++)
    {
        if (arr[i-1] > arr[i]) return false;
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
        int k;
        cin >> k;

        vector<int> arr(n);
        for (int i=0; i<n; i++) cin >> arr[i];

        if (f(arr, k)) cout << "YES" << endl;
        else cout << "NO" << endl; 
    }

    return 0;
}