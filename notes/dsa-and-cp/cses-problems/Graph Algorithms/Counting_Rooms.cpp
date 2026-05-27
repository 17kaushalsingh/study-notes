#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int mod = (int)(1e9 + 7);

const int dr[4] = {0, 0, 1, -1};
const int dc[4] = {1, -1, 0, 0};

bool valid(int row, int col, int n, int m)
{
    return (row >= 0 && row < n && col >= 0 && col < m);
}

void bfs(int row, int col, vector<vector<char>> &grid, vector<vector<bool>> &vis)
{
    queue<pair<int, int>> q;
    q.push({row, col});
    vis[row][col] = true;

    while (!q.empty())
    {
        pair<int, int> xy = q.front();
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int nr = xy.first + dr[i];
            int nc = xy.second + dc[i];

            if (!valid(nr, nc, grid.size(), grid[0].size()))
                continue;
            if (grid[nr][nc] != '.')
                continue;
            if (vis[nr][nc])
                continue;

            q.push({nr, nc});
            vis[nr][nc] = true;
        }
    }
}

void solve()
{
    int n, m;
    cin >> n >> m;

    vector<vector<char>> grid(n, vector<char>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> grid[i][j];
        }
    }

    int rooms = 0;
    vector<vector<bool>> vis(n, vector<bool>(m, false));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (!vis[i][j] && grid[i][j] == '.')
            {
                bfs(i, j, grid, vis);
                rooms++;
            }
        }
    }

    cout << rooms << endl;
}

#undef int
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    while (t--)
    {
        solve();
    }
    return 0;
}
