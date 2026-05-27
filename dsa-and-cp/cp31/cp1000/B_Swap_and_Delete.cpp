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

int getCnt(string &s, string &t, int n) {
    int i = 0;
    int j = 0;

    int cnt = 0;
    while(i < n && j < n) {
        if (s[i] != t[i]) {
            i++;
            j++;
        } else {
            cnt++;
            j++;
        }
    }

    return cnt;
}

void solve() {
    string s; cin >> s;

    int n = s.size();

    if (n == 1) {
        cout << 1 << endl;
        return;
    }

    int zeros = 0;
    for (int i=0; i<n; i++) if (s[i] == '0') zeros++;
    int swaps = min(zeros, n-zeros);

    string t = "";
    for (int i=0; i<n; i++) {
        if (s[i] == '1' && swaps > 0) {
            t += '0';
            swaps--;
        } else if (s[i] == '1' && !swaps) {
            t += '1';
        } else if (s[i] == '0') t += '0';
    }

    swaps = min(zeros, n-zeros);
    for (int i=0; i<n; i++) {
        if (s[i] == '0' && swaps > 0) {
            t[i] = '1';
            swaps--;
        }
    }

    int cnt = getCnt(s, t, n);

    cout << cnt << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) solve();
    return 0;
}