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

vector<int> getPath(vector<int> &parent, int startNode) {
    vector<int> path;

    int node = startNode;
    path.push_back(node);

    while(parent[node] != node) {
        int prevNode = parent[node];
        path.push_back(prevNode);
        node = prevNode;
    }

    reverse(all(path));
    return path;
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

    queue<int> q;
    vector<int> parent(n+1, -1);

    q.push(1);
    parent[1] = 1;

    while(!q.empty()) {
        int node = q.front();
        q.pop();

        if (node == n) {
            vector<int> path = getPath(parent, node);
            cout << path.size() << endl;
            for (int &x : path) cout << x << " ";
            cout << endl;
            return;
        }

        for (int &adjNode : adj[node]) {
            if (parent[adjNode] == -1) {
                q.push(adjNode);
                parent[adjNode] = node;
            }
        }
    }

    cout << "IMPOSSIBLE" << endl;
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
