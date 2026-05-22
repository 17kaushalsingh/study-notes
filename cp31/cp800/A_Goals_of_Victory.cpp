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

int f(vector<int> &arr)
{
    long long int sum = 0;
    for (int x : arr) sum += x;
    
    return (-1*sum);
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
        vector<int> arr(n-1);
        for (int i=0; i<n-1; i++) cin >> arr[i];

        int ans = f(arr);
        cout << ans << endl;
    }

    return 0;
}