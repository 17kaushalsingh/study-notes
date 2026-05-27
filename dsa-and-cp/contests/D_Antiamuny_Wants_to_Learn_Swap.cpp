#include <bits/stdc++.h>
using namespace std;

struct SparseTable {
    int n, K;
    vector<vector<int>> st;
    vector<int> lg;
    function<int(int,int)> func;

    SparseTable() {}
    SparseTable(const vector<int>& arr, function<int(int,int)> f) {
        n = arr.size();
        func = f;
        K = 32 - __builtin_clz(n);
        st.assign(K, vector<int>(n));
        lg.assign(n+1, 0);

        for (int i = 2; i <= n; i++) lg[i] = lg[i/2] + 1;
        for (int i = 0; i < n; i++) st[0][i] = arr[i];

        for (int k = 1; k < K; k++) {
            for (int i = 0; i + (1<<k) <= n; i++) {
                st[k][i] = func(st[k-1][i], st[k-1][i + (1<<(k-1))]);
            }
        }
    }

    int query(int L, int R) {
        int j = lg[R-L+1];
        return func(st[j][L], st[j][R - (1<<j) + 1]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        // build sparse tables
        SparseTable stMin(a, [](int x, int y){ return min(x,y); });
        SparseTable stMax(a, [](int x, int y){ return max(x,y); });

        while (q--) {
            int l, r;
            cin >> l >> r;
            --l; --r; // 0-based
            int subMin = stMin.query(l, r);
            int subMax = stMax.query(l, r);

            if ((a[l] == subMin || a[l] == subMax) &&
                (a[r] == subMin || a[r] == subMax)) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }
    return 0;
}
