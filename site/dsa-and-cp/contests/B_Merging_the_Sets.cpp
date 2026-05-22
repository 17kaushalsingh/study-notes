#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class DisjointSet
{
private:
    vector<ll> parent, rank, size;
    ll n;

public:
    DisjointSet(ll n)
    {
        this->n = n;
        parent.resize(n + 1);
        rank.resize(n + 1);
        size.resize(n + 1);
        for (ll i = 0; i <= n; i++)
        {
            parent[i] = i;
            rank[i] = 0;
            size[i] = 1;
        }
    }

    ll findUltimateParent(ll node)
    {
        if (parent[node] == node)
            return node;
        return parent[node] = findUltimateParent(parent[node]);
    }

    void unionByRank(ll u, ll v)
    {
        ll ulp_u = findUltimateParent(u);
        ll ulp_v = findUltimateParent(v);
        if (ulp_u == ulp_v)
            return;
        if (rank[ulp_u] < rank[ulp_v])
            parent[ulp_u] = ulp_v;
        else if (rank[ulp_u] > rank[ulp_v])
            parent[ulp_v] = ulp_u;
        else
        {
            parent[ulp_u] = ulp_v;
            rank[ulp_v]++;
        }
    }

    void unionBySize(ll u, ll v)
    {
        ll ulp_u = findUltimateParent(u);
        ll ulp_v = findUltimateParent(v);
        if (ulp_u == ulp_v)
            return;
        if (size[ulp_u] < size[ulp_v])
        {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        }
        else
        {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }

    bool areConnected(ll u, ll v)
    {
        return (findUltimateParent(u) == findUltimateParent(v));
    }

    ll getRank(ll node)
    {
        return rank[findUltimateParent(node)];
    }

    ll getSize(ll node)
    {
        return size[findUltimateParent(node)];
    }
};

vector<ll> takeInput(ll n)
{
    vector<ll> arr(n);
    for (ll i = 0; i < n; i++)
        cin >> arr[i];
    return arr;
}

void printArray(vector<ll> &arr)
{
    for (ll &x : arr)
        cout << x << " ";
    cout << "\n";
}

ll getMin(vector<ll> &arr)
{
    ll mini = LLONG_MAX;
    for (ll &x : arr)
        mini = min(mini, x);
    return mini;
}

ll getMax(vector<ll> &arr)
{
    ll maxi = LLONG_MIN;
    for (ll &x : arr)
        maxi = max(maxi, x);
    return maxi;
}

ll getSum(vector<ll> &arr)
{
    ll sum = 0;
    for (ll &x : arr)
        sum += x;
    return sum;
}

void solve()
{
    ll n, m;
    cin >> n >> m;

    vector<vector<ll>> S(n);
    ll q = 0; // no of sets not used in the union formation
    map<ll, ll> freq;
   
    for (ll i = 0; i < n; i++)
    {
        ll l;
        cin >> l;
        S[i].resize(l, 0);

        for (ll j = 0; j < l; j++)
        {
            cin >> S[i][j];
            freq[S[i][j]]++;
        }

    }

    // NO , at least one element is missing
    for (ll i=1; i<=m; i++) {
        if (freq[i] == 0) {
            cout << "NO" << endl;
            return;
        }
    }

    for (ll i=0; i<n; i++) {
        if (S[i].size() >= m) {
            cout << "YES" << endl;
            return;
        }
    }

    bool redundant = true;
    for (ll i=1; i<=m; i++) {
        if (freq[i] < 2) {
            redundant = false;
            break;
        }
    }

    if (redundant) {
        cout << "YES" << endl;
        return;
    }

    cout << "NO" << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--)
        solve();
    return 0;
}