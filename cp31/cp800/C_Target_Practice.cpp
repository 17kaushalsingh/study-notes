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


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
    {
        int score = 0;
        vector<int> valRow = {1,2,3,4,5,5,4,3,2,1};
        vector<int> valCol = {1,2,3,4,5,5,4,3,2,1};

        for (int i=0; i<10; i++)
        {
            for (int j=0; j<10; j++)
            {
                char temp;
                cin >> temp;
                if (temp == 'X') score += min(valRow[i], valCol[j]);
            }
        }

        cout << score << endl;

    }

    return 0;
}