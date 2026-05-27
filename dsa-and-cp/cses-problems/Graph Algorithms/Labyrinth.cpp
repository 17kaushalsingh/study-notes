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

bool valid(int row, int col, int n, int m) {
    return (row >= 0 && row < n && col >= 0 && col < m);
}

int dr[4] = {1,-1,0,0};
int dc[4] = {0,0,-1,1};

string getPath(vector<vector<pair<int, int>>> &parent, int startRow, int startCol) {
    string path = "";

    int row = startRow;
    int col = startCol;
    while(true) {
        int prevRow = parent[row][col].first;
        int prevCol = parent[row][col].second;

        if (prevRow == row && prevCol == col) break;

        if (prevRow - row == 1) path += 'U';
        else if (prevRow - row == -1) path += 'D';
        else if (prevCol - col == 1) path += 'L';
        else if (prevCol - col == -1) path += 'R';

        row = prevRow;
        col = prevCol;
    }

    reverse(all(path));
    return path;
}

void solve() {
    int n, m; 
    cin >> n >> m;
    vector<vector<char>> grid(n, vector<char>(m));
    pair<int, int> A, B;

    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            cin >> grid[i][j];

            if (grid[i][j] == 'A') A = {i, j};
            else if (grid[i][j] == 'B') {
                B = {i, j};
                grid[i][j] = '.';
            }
        }
    }

    // queue<pair<pair<int, int>, string>> q;
    // q.push({{A.first, A.second}, ""});
    // grid[A.first][A.second] = '#';


    // while(!q.empty()) {
    //     auto it = q.front();
    //     q.pop();

    //     int x = it.first.first;
    //     int y = it.first.second;
    //     string path = it.second;

    //     if (x == B.first && y == B.second) {
    //         cout << "YES" << endl;
    //         cout << path.size() << endl;
    //         cout << path << endl;
    //         return;
    //     }

    //     for (int i=0; i<4; i++) {
    //         int nr = x + dr[i];
    //         int nc = y + dc[i];

    //         if (valid(nr, nc, n, m) && grid[nr][nc] == '.') {
    //             if (i == 0) q.push({{nr, nc}, path+'D'});
    //             else if (i == 1) q.push({{nr, nc}, path+'U'});
    //             else if (i == 2) q.push({{nr, nc}, path+'L'});
    //             else if (i == 3) q.push({{nr, nc}, path+'R'});
                
    //             grid[nr][nc] = '#';
    //         }
    //     }
    // }

    // cout << "NO" << endl;


    queue<pair<int, int>> q;
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, {-1, -1}));
    q.push({A.first, A.second});
    parent[A.first][A.second] = {A.first, A.second};

    while(!q.empty()) {
        auto it = q.front(); q.pop();

        if (it == B) {
            cout << "YES" << endl;
            string path = getPath(parent, it.first, it.second);
            cout << path.size() << endl;
            cout << path << endl;
            return;
        }

        int row = it.first;
        int col = it.second;

        for (int i=0; i<4; i++) {
            int nr = row + dr[i];
            int nc = col + dc[i];

            if (!valid(nr, nc, n, m)) continue;
            if (grid[nr][nc] != '.') continue;
            if (parent[nr][nc].first == -1 && parent[nr][nc].second == -1) {
                q.push({nr, nc});
                parent[nr][nc] = make_pair(row, col);
            }
        }
    }

    cout << "NO" << endl;
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
