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
#
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        int n, x;
        cin >> n >> x;

        vector<int> arr(n);
        for (int i=0; i<n; i++) cin >> arr[i];

        int maxi = INT_MIN;
        maxi = max(maxi, arr[0]);
        for (int i=0; i<n-1; i++)
        {
            maxi = max(maxi, arr[i+1] - arr[i]);
        }
        maxi = max(maxi, 2*(x-arr[n-1]));

        cout << maxi << endl;
    }
    
    return 0;
}