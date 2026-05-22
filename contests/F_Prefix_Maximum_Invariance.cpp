#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define vi vector<int>
#define vll vector<ll>
#define pb push_back
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x.size())
#define F first
#define S second

int solve_f(vi& x, vi& y) {
    int m = sz(x);
    if (m == 0) return 0;
    
    int count = 0;
    int current_max = x[0];
    
    // First position: z[0] must equal x[0]
    if (x[0] == y[0]) count++;
    
    // Process remaining positions
    for (int i = 1; i < m; i++) {
        if (x[i] > current_max) {
            // Prefix maximum increases, so z[i] must equal x[i]
            current_max = x[i];
            if (x[i] == y[i]) count++;
        } else {
            // Prefix maximum doesn't increase, z[i] can be y[i] if y[i] <= current_max
            if (y[i] <= current_max) count++;
        }
    }
    
    return count;
}

void solve() {
    int n;
    cin >> n;
    
    vi a(n), b(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) cin >> b[i];
    
    ll total = 0;
    
    // Iterate over all subarrays [l, r]
    for (int l = 0; l < n; l++) {
        for (int r = l; r < n; r++) {
            vi x(a.begin() + l, a.begin() + r + 1);
            vi y(b.begin() + l, b.begin() + r + 1);
            total += solve_f(x, y);
        }
    }
    
    cout << total << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        solve();
    }
    
    return 0;
}