#include<bits/stdc++.h>
using namespace std;

vector<long long> getAllBinaryPalindromes() {
    vector<long long> result;
    
    // We'll generate palindromes up to 2^32
    // Maximum length is 32 bits
    
    for (int len = 1; len <= 32; len++) {
        int halfLen = (len + 1) / 2;
        // First bit must be 1, so firstHalf ranges from 2^(halfLen-1) to 2^halfLen - 1
        long long start = (1LL << (halfLen - 1));
        long long end = (1LL << halfLen) - 1;
        
        for (long long firstHalf = start; firstHalf <= end; firstHalf++) {
            // Convert firstHalf to binary string
            string firstStr = "";
            long long temp = firstHalf;
            while (temp) {
                firstStr += char('0' + (temp % 2));
                temp /= 2;
            }
            
            // Create palindrome
            string palindrome = firstStr;
            // Add the reverse of the first (len - halfLen) characters
            string suffix = firstStr.substr(0, len - halfLen);
            reverse(suffix.begin(), suffix.end());
            palindrome += suffix;
            
            // Convert back to number
            long long num = 0;
            for (char c : palindrome) {
                num = num * 2 + (c - '0');
            }
            
            // Only consider if it fits in our range (should always be true)
            if (num > 0 && num <= (1LL << 32)) {
                result.push_back(num);
            }
        }
    }
    
    // Add 0 as well (binary "0" is palindrome)
    result.push_back(0);
    
    sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    return result;
}

vector<long long> palindromes = getAllBinaryPalindromes();

int main() {
	int t; cin >> t;
	while(t--) {
        long long n; cin >> n;
        
        // Binary search for closest palindrome
        auto it = lower_bound(palindromes.begin(), palindromes.end(), n);
        long long minOps = LLONG_MAX;
        
        // Check palindrome >= n
        if (it != palindromes.end()) {
            minOps = min(minOps, *it - n);
        }
        
        // Check palindrome < n  
        if (it != palindromes.begin()) {
            --it;
            minOps = min(minOps, n - *it);
        }
        
        cout << minOps << '\n';
	}

	return 0;
}

