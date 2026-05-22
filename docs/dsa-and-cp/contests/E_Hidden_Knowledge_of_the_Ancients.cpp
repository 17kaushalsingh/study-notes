#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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

// ll f(vector<ll> &a, ll n, ll k, ll len)
// {
//     ll i = 0;
//     ll j = len-1;
//     ll cnt = 0;
//     while(j < n) {

//         unordered_set<ll> st;
//         for (ll k=i; k<=j; k++) st.insert(a[k]);
//         if (st.size() == k) cnt++;

//         i++;
//         j++;
//     }

//     return cnt;
// }

// void solve() {
//     ll n,k,l,r; cin >> n >> k >> l >>r;
//     vector<ll> a = takeInput(n);

//     ll cnt = 0;
//     for (ll winLen = l; winLen <= r; winLen++) {
//         cnt += f(a, n, k, winLen);
//     }

//     cout << cnt << endl;
// }

bool check(ll len, ll l, ll r) {
    return (len >= l && len <= r);
}

void solve() {
    ll n,k,l,r; cin >> n >> k >> l >>r;
    vector<ll> a = takeInput(n);

    ll i = 0;
    ll j = l-1;
    map<ll, ll> freq;
    for (ll x=i; x<=j; x++) freq[a[x]]++;

    ll cnt = 0;
    while(j < n) {
        ll len = j-i+1;

        if (check(len, l, r) && freq.size() == k) cnt++;

        while(len < r && j+1 < n) {
            j++;
            len = j-i+1;
            freq[a[j]]++;
            if (check(len, l, r) && freq.size() == k) cnt++;
        }

        while(len > l) {
            freq[a[i]]--;
            if (freq[a[i]] <= 0) freq.erase(a[i]);
            i++;
            len = j-i+1;

            if (check(len, l, r) && freq.size() == k) cnt++;
        }

        j++;
        if (j < n) freq[a[j]]++;
    }

    cout << cnt << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) solve();
    return 0;
}