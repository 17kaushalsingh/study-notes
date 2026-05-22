#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

typedef long long ll;

// Compute alternating sum for array a
ll alt_sum(const vector<ll>& a) {
    ll s = 0;
    for (int i = 0; i < a.size(); i++) {
        if (i % 2 == 0) s += a[i];
        else s -= a[i];
    }
    return s;
}

// Bob's best response: from state (a, cost), he picks move to minimize f
// He can end, or do one swap
ll bob_best_response(vector<ll> a, ll cost) {
    int n = a.size();
    ll current_f = cost + alt_sum(a);
    ll best_f = current_f; // at least he can end

    // Try all swaps for Bob
    for (int l = 0; l < n; l++) {
        for (int r = l; r < n; r++) {
            if (l == r) continue;

            vector<ll> new_a = a;
            swap(new_a[l], new_a[r]);
            ll new_cost = cost + (r - l);
            ll new_f = new_cost + alt_sum(new_a);

            if (new_f < best_f) {
                best_f = new_f;
            }
        }
    }

    return max(best_f, alt_sum(a));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;
        vector<ll> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        ll f0 = alt_sum(a); // Alice can end immediately

        ll best_outcome = f0; // at least this

        // Alice considers every possible swap
        for (int l = 0; l < n; l++) {
            for (int r = l; r < n; r++) {
                if (l == r) continue;

                vector<ll> new_a = a;
                swap(new_a[l], new_a[r]);
                ll new_cost = (r - l);
                // Bob responds optimally to this state
                ll outcome = bob_best_response(new_a, new_cost);

                if (outcome > best_outcome) {
                    best_outcome = outcome;
                }
            }
        }

        cout << best_outcome << '\n';
    }

    return 0;
}