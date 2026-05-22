// #include <bits/stdc++.h>
// #define ll long long
// #define all(x) (x).begin(), (x).end()
// using namespace std;

// static bool comp(const vector<int> &v1, const vector<int> &v2) {
//     return v1.size() < v2.size();
// }

// void solve() {
//     int n; cin >> n;
//     vector<vector<int>> arr(n);
//     for (int i=0; i<n; i++) {
//         int k; cin >> k;
//         arr[i].resize(k);
//         for (int j=0; j<k; j++) cin >> arr[i][j];
//     }

//     sort(arr.begin(), arr.end(), comp);
//     int m = arr.back().size();
//     vector<int> ans(m, INT_MAX);
//     set<int> edits; edits.insert(0);

//     for (vector<int> &v : arr) {
//         int k = v.size();
//         auto it = edits.begin();

//         while(it != edits.end() && *it < k) {
//             int l = *it;
//             auto it2 = next(it);
//             int r = (it2 == edits.end()) ? k : *it2;

//             bool replace = false;
//             for (int i=l; i<r; i++) {
//                 if (ans[i] < v[i]) break;
//                 else if (v[i] < ans[i]) {
//                     replace = true;
//                     break;
//                 }
//             }

//             if (replace) {
//                 int up = min(k, m);
//                 for (int i=l; i<up; i++) ans[i] = v[i];

//                 auto it_del = edits.find(l);
//                 if (it_del != edits.end()) {
//                     ++it_del; // first element > l
//                     edits.erase(it_del, edits.end());
//                 }
//                 // add new breakpoint at k (if k <= m)
//                 edits.insert(k);
//                 break; // done with this array
//             }

//             it = it2;
//         }
//     }

//     for (auto &x : ans) cout << x << " ";
//     cout << endl;
// }

// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);

//     int t; cin >> t;
//     while (t--) solve();
//     return 0;
// }

#include <bits/stdc++.h>
#define ll long long
#define all(x) (x).begin(), (x).end()
using namespace std;

static bool comp(const vector<int> &v1, const vector<int> &v2) {
    return v1.size() < v2.size();
}

void solve() {
    int n; cin >> n;
    vector<vector<int>> arr(n);
    for (int i=0; i<n; i++) {
        int k; cin >> k;
        arr[i].resize(k);
        for (int j=0; j<k; j++) cin >> arr[i][j];
    }

    sort(arr.begin(), arr.end(), comp);
    int m = arr.back().size();
    vector<int> ans(m, INT_MAX);
    set<int> edits; edits.insert(0);

    for (vector<int> &v : arr) {
        int k = v.size();
        auto it = edits.begin();

        while(it != edits.end() && *it < k) {
            auto it2 = next(it);
            int l = *it;
            int r = (it2 == edits.end()) ? k : *it2;

            bool replace = false;
            for (int i=l; i<r; i++) {
                if (ans[i] < v[i]) break;
                else if (v[i] < ans[i]) {
                    replace = true;
                    break;
                }
            }

            if (replace) {
                int up = min(k, m);
                for (int i=l; i<up; i++) ans[i] = v[i];

                auto it_del = edits.find(l);
                if (it_del != edits.end()) {
                    ++it_del;
                    edits.erase(it_del, edits.end());
                }

                edits.insert(k);
                break;
            }

            it = it2;
        }
    }

    for (auto &x : ans) cout << x << " ";
    cout << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) solve();
    return 0;
}