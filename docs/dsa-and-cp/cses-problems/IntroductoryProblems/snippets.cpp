int exp(int x, int y) {
    // comutes x ^ y

    int res = 1;
    while(y > 0) {
        if (y & 1) {
            res = res * x % mod;
            y--;
        } else {
            x = x * x % mod;
            y = y >> 1;
        }
    }

    return res;
}

