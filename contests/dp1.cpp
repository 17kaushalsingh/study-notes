#include <bits/stdc++.h>
using namespace std;

int calculateSchedulingTime(vector<int>& capacity, long requests) {
    // Max-heap to always pick the server with the largest capacity
    priority_queue<int> pq;
    for (int cap : capacity) {
        if (cap > 0) pq.push(cap);
    }

    long remaining = requests;
    int time = 0;

    while (remaining > 0 && !pq.empty()) {
        int current = pq.top();
        pq.pop();

        // Process one request
        remaining--;
        time++;

        // Update server capacity
        int nextCap = current / 2;
        if (nextCap > 0) pq.push(nextCap);
    }

    // If all servers are exhausted but requests remain
    // if (remaining > 0) {
    //     // Each request will take 1 second since capacities are now 0
    //     time += remaining;
    // }

    return time;
}

int main() {
    vector<int> capacity = {2, 8, 3, 9, 5};
    long requests = 17;
    cout << calculateSchedulingTime(capacity, requests) << endl; // Output: 9
    return 0;
}
