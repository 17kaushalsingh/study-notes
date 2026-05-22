#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int mod = (int)(1e9+7);

vector<int> takeInput(int n) {
    vector<int> arr(n);
    for (int &x : arr) cin >> x;
    return arr;
}

void printArray(vector<int> &arr) {
    for (int &x : arr) cout << x << " ";
    cout << "\n";
}

class Base {
public:
    virtual void print() = 0;
};

class Derived : public Base{
public:
    void print() override {
        cout << "Derived" << endl;
    }
};


#undef int
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x;
    int y;
    cout << "x : " << x << endl; 
    cout << "y : " << y << endl; 

    return 0;
}
