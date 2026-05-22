#include<bits/stdc++.h>
using namespace std;

int main() {
    string s;
    cin >> s;
    int n = s.length();
    
    vector<int> doubles;
    for (int i = 0; i < n - 1; i++) {
        if (s[i] == s[i + 1]) {
            doubles.push_back(i);
        }
    }
    
    if (doubles.empty()) {
        cout << 0 << endl;
        return 0;
    }
    
    long long operations = 0;
    int j = 0; // pointer to doubles array
    
    for (int i=2; i<n; i++) {
        while(j < doubles.size() && doubles[j] <= i-2) j++;

        int cnt = j;

        if (cnt == 0) continue;

        if (cnt == 1 && s[i] == s[doubles[0]]) {

        } else operations += cnt;

        // cout << operations << " ";
    }
    
    cout << operations << endl;
    return 0;
}