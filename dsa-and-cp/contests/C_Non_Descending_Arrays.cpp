#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll mod = 998244353;

class DisjointSet {
private:
    vector<ll> parent, rank, size;
    ll n;

public:
    DisjointSet(ll n) {
        this->n = n;
        parent.resize(n+1);
        rank.resize(n+1);
        size.resize(n+1);
        for (ll i=0; i<=n; i++) {
            parent[i] = i;
            rank[i] = 0;
            size[i] = 1;
        }
    }

    ll findUltimateParent(ll node) {
        if (parent[node] == node) return node;
        return parent[node] = findUltimateParent(parent[node]);
    }

    void unionByRank(ll u, ll v) {
        ll ulp_u = findUltimateParent(u);
        ll ulp_v = findUltimateParent(v);
        if (ulp_u == ulp_v) return;
        if (rank[ulp_u] < rank[ulp_v]) parent[ulp_u] = ulp_v;
        else if (rank[ulp_u] > rank[ulp_v]) parent[ulp_v] = ulp_u;
        else {
            parent[ulp_u] = ulp_v;
            rank[ulp_v]++;
        }
    }

    void unionBySize(ll u, ll v) {
        ll ulp_u = findUltimateParent(u);
        ll ulp_v = findUltimateParent(v);
        if (ulp_u == ulp_v) return;
        if (size[ulp_u] < size[ulp_v]) {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        } else {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }

    bool areConnected(ll u, ll v) {
        return (findUltimateParent(u) == findUltimateParent(v));
    }

    ll getRank(ll node) {
        return rank[findUltimateParent(node)];
    }

    ll getSize(ll node) {
        return size[findUltimateParent(node)];
    }
};

vector<ll> takeInput(ll n) {
    vector<ll> arr(n);
    for (ll i=0; i<n; i++) cin >> arr[i];
    return arr;
}

void printArray(vector<ll> &arr) {
    for (ll &x : arr) cout << x << " ";
    cout << "\n";
}

ll getMin(vector<ll> &arr) {
    ll mini = LLONG_MAX;
    for (ll &x : arr) mini = min(mini, x);
    return mini;
}

ll getMax(vector<ll> &arr) {
    ll maxi = LLONG_MIN;
    for (ll &x : arr) maxi = max(maxi, x);
    return maxi;
}

ll getSum(vector<ll> &arr) {
    ll sum = 0;
    for (ll &x : arr) sum += x;
    return sum;
}

void solve() {
    ll n;
    cin >> n;
    vector<ll> a = takeInput(n);
    vector<ll> b = takeInput(n);

    // dp[i][c] for i in [0, n-1], c in {0,1}
    vector<vector<ll>> dp(n, vector<ll>(2, 0));

    // Base case: index 0
    dp[0][0] = 1; // no swap
    dp[0][1] = 1; // swap

    for (ll i = 1; i < n; i++) {
        for (ll curr_choice = 0; curr_choice < 2; curr_choice++) {
            ll curr_a = (curr_choice == 0) ? a[i] : b[i];
            ll curr_b = (curr_choice == 0) ? b[i] : a[i];

            for (ll prev_choice = 0; prev_choice < 2; prev_choice++) {
                ll prev_a = (prev_choice == 0) ? a[i-1] : b[i-1];
                ll prev_b = (prev_choice == 0) ? b[i-1] : a[i-1];

                if (curr_a >= prev_a && curr_b >= prev_b) {
                    dp[i][curr_choice] = (dp[i][curr_choice] + dp[i-1][prev_choice]) % mod;
                }
            }
        }
    }

    ll ans = (dp[n-1][0] + dp[n-1][1]) % mod;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) solve();
    return 0;
}