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

class DisjointSet {
    vector<int> parent;
    vector<int> size;

    int getParent(int node) {
        if (node == parent[node]) return node;
        return parent[node] = getParent(parent[node]);
    }

public:
    DisjointSet(int n) {
        parent.resize(n);
        for (int i=0; i<n; i++) parent[i] = i;
        size.resize(n, 1);
    }

    void unionBySize(int u, int v) {
        int ulp_u = getParent(u);
        int ulp_v = getParent(v);

        if (ulp_u == ulp_v) return;

        if (size[ulp_u] <= size[ulp_v]) {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        } else {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }

    bool areConnected(int u, int v) {
        int ulp_u = getParent(u);
        int ulp_v = getParent(v);

        return (ulp_u == ulp_v);
    }

    bool isUltimateParent(int node) {
        int ulp = getParent(node);
        return (ulp == node);
    }
};

void solve() {
    int n, m;
    cin >> n >> m;

    DisjointSet ds(n+1);

    for (int i=0; i<m; i++) {
        int u, v;
        cin >> u >> v;

        if (!ds.areConnected(u, v)) ds.unionBySize(u, v);
    }

    vector<int> ultimateParents;
    for (int i=1; i<=n; i++) {
        if (ds.isUltimateParent(i)) ultimateParents.push_back(i);
    }

    cout << ultimateParents.size() - 1 << endl;
    for (int i=0; i<ultimateParents.size()-1; i++) {
        cout << ultimateParents[i] << " " << ultimateParents[i+1] << endl;
    }
}

#undef int
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    while (t--) {
        solve();
    }
    return 0;
}
