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

bool checkForCycle(int node, int parent, vector<vector<int>> &adj, vector<bool> &vis) {
    vis[node] = true;

    for (int &adjNode : adj[node]) {
        if (adjNode == parent) continue;
        if (!vis[adjNode]) {
            if (checkForCycle(adjNode, node, adj, vis)) return true;
        } return true;
    }

    return false;
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n+1);
    for (int i=0; i<m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<bool> vis(n+1, false);
    for (int i=1; i<=n; i++) {
        if (!vis[i]) {
            bool isCycle = checkForCycle(i, -1, adj, vis);
            if (isCycle) {
                vector<int> nodes = getNodes(i, adj);
                if (nodes.size())
            }
        }
    }

    cout << "IMPOSSIBLE" << endl;
}

#undef int
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
