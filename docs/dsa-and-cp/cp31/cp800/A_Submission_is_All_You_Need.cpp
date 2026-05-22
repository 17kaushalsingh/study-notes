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

int solve(set<int> &st)
{
    int sum = 0;
    for (int num : st) sum += num;

    int mex = 0;
    for (int i=0; i<INT_MAX; i++) 
    {
        if (st.find(i) == st.end()) 
        {
            mex = i;
            break;
        }
    }

    int ans = sum + mex;
    return ans;
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
        set<int> st;
        for (int i=0; i<n; i++)
        {
            int temp;
            cin >> temp;
            st.insert(temp);
        }

        cout << solve(st) << endl;
    }
    return 0;
}