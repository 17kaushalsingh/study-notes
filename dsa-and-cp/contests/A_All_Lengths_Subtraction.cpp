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

bool f(vector<ll> &p, ll n) {
    priority_queue<pair<ll, pair<ll, ll>>> pq;
    for (ll i=0; i<n; i++) {
        pq.push({p[i], {i, i}});
    }

    while(pq.size() >= 2) {
        auto it1 = pq.top();
        ll l = it1.second.first;
        ll u = it1.second.second;
        pq.pop();

        auto it2 = pq.top();
        pq.pop();

        if (it2.second.first == l-1) {
            it2.second.first = l-1;
            it2.second.second = u;
            pq.push(it2);
        }

        else if (it2.second.first == u+1) {
            it2.second.first = l;
            it2.second.second = u+1;
            pq.push(it2);
        }

        else return false;
    }

    return true;
}

void solve() {
    ll n; cin >> n;
    vector<ll> p = takeInput(n);

    bool ans = f(p, n);

    if (ans) cout << "YES" << endl;
    else cout << "NO" << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) solve();
    return 0;
}