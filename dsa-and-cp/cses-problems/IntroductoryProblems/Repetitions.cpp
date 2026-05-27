#include<iostream>
 
int main() {
    std::string str; std::cin >> str;
 
    int n = str.size();

    int len = 1;
    int maxLen = 1;
 
    for (int i=1; i<n; i++)
    {
        if (str[i] == str[i-1]) {
            len++;
        }
 
        else {
            len = 1;
        }
 
        maxLen = std::max(maxLen, len);
    }
 
    std::cout << maxLen;
    
    return 0;
}