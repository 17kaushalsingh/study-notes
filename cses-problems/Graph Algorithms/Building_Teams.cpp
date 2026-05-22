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

bool bfs(int node, vector<vector<int>> &adj, vector<int> &color) {
    queue<pair<int, int>> q; // node, color

    color[node] = 1;
    q.push({node, 1});

    while(!q.empty()) {
        auto it = q.front();
        q.pop();
        int node = it.first;
        int nodeColor = it.second;

        for (int &adjNode : adj[node]) {
            if (color[adjNode] != -1) {
                if (color[adjNode] == nodeColor) {
                    return false;
                }
            } else {
                int adjNodeColor = (nodeColor == 1) ? 2 : 1;
                q.push({adjNode, adjNodeColor});
                color[adjNode] = adjNodeColor;
            }
        }
    }

    return true;
}

void solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n+1);
    vector<int> color(n+1, -1);

    for (int i=0; i<m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i=1; i<=n; i++) {
        if (color[i] == -1) {
            bool isBipartite = bfs(i, adj, color);
            if (!isBipartite) {
                cout << "IMPOSSIBLE" << endl;
                return;
            }
        }
    }

    for (int i=1; i<=n; i++) cout << color[i] << " ";
    cout << endl;
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
