#include <bits/stdc++.h>
using namespace std;
#define int long long
#define all(x) (x).begin(), (x).end()

vector<int> takeInput(int n) {
    vector<int> arr(n);
    for (int i=0; i<n; i++) cin >> arr[i];
    return arr;
}

void printArray(vector<int> &arr) {
    for (int &x : arr) cout << x << " ";
    cout << "\n";
}

void solve() {
    
}

class SegmentTree {
private:
    int n;
    vector<int> sumTree;   // for range sum
    vector<int> minTree;   // for range min
    vector<int> maxTree;   // for range max
    vector<int> arr;

    void build(int idx, int l, int r) {
        if (l == r) {
            sumTree[idx] = arr[l];
            minTree[idx] = arr[l];
            maxTree[idx] = arr[l];
            return;
        }

        int mid = (l + r) / 2;
        build(2 * idx, l, mid);
        build(2 * idx + 1, mid + 1, r);

        sumTree[idx] = sumTree[2 * idx] + sumTree[2 * idx + 1];
        minTree[idx] = min(minTree[2 * idx], minTree[2 * idx + 1]);
        maxTree[idx] = max(maxTree[2 * idx], maxTree[2 * idx + 1]);
    }

    long long querySum(int idx, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return sumTree[idx];
        int mid = (l + r) / 2;
        return querySum(2 * idx, l, mid, ql, qr) + querySum(2 * idx + 1, mid + 1, r, ql, qr);
    }

    long long queryMin(int idx, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return LLONG_MAX;
        if (ql <= l && r <= qr) return minTree[idx];
        int mid = (l + r) / 2;
        return min(queryMin(2 * idx, l, mid, ql, qr), queryMin(2 * idx + 1, mid + 1, r, ql, qr));
    }

    int queryMax(int idx, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return LLONG_MIN;
        if (ql <= l && r <= qr) return maxTree[idx];
        int mid = (l + r) / 2;
        return max(queryMax(2 * idx, l, mid, ql, qr), queryMax(2 * idx + 1, mid + 1, r, ql, qr));
    }

    void update(int idx, int l, int r, int pos, int val) {
        if (l == r) {
            sumTree[idx] = val;
            minTree[idx] = val;
            maxTree[idx] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(2 * idx, l, mid, pos, val);
        else update(2 * idx + 1, mid + 1, r, pos, val);

        sumTree[idx] = sumTree[2 * idx] + sumTree[2 * idx + 1];
        minTree[idx] = min(minTree[2 * idx], minTree[2 * idx + 1]);
        maxTree[idx] = max(maxTree[2 * idx], maxTree[2 * idx + 1]);
    }

public:
    SegmentTree(vector<int>& input) {
        arr = input;
        n = arr.size();
        sumTree.assign(4 * n, 0);
        minTree.assign(4 * n, LLONG_MAX);
        maxTree.assign(4 * n, LLONG_MIN);
        build(1, 0, n - 1);
    }

    int rangeSum(int l, int r) { 
        return querySum(1, 0, n - 1, l, r); 
    }
    int rangeMin(int l, int r) { 
        return queryMin(1, 0, n - 1, l, r); 
    }
    int rangeMax(int l, int r) { 
        return queryMax(1, 0, n - 1, l, r); 
    }
    void pointUpdate(int pos, int val) { 
        update(1, 0, n - 1, pos, val); 
    }
};


#undef int
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}