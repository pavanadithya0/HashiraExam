#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>

#define MAX_POINTS 20
typedef long long ll;

struct Point {
    ll x;
    ll y;
};

ll decode_base(const std::string& str, int base) {
    ll value = 0;
    for (char c : str) {
        value = value * base + (c - '0');
    }
    return value;
}

ll modInverse(ll a, ll m) {
    ll m0 = m, t, q;
    ll x0 = 0, x1 = 1;
    while (a > 1) {
        q = a / m;
        t = m; m = a % m; a = t;
        t = x0; x0 = x1 - q * x0; x1 = t;
    }
    return x1 < 0 ? x1 + m0 : x1;
}

ll lagrange(Point points[], int k, ll prime = 7919) {
    ll result = 0;
    for (int i = 0; i < k; ++i) {
        ll xi = points[i].x;
        ll yi = points[i].y;
        ll num = 1, den = 1;
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                ll xj = points[j].x;
                num = (num * (prime - xj)) % prime;
                den = (den * (xi - xj + prime)) % prime;
            }
        }
        ll inv = modInverse(den, prime);
        ll term = (yi * num % prime) * inv % prime;
        result = (result + term) % prime;
    }
    return result;
}

int read_testcase(const std::string& filename, Point points[], int &k) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file " << filename << "\n";
        return -1;
    }

    std::string key;
    int n = 0;
    while (file >> key) {
        if (key == "keys") {
            std::string nk;
            int val;
            file >> nk >> val;
            if (nk == "n") n = val;
            else if (nk == "k") k = val;
        } else {
            int x = std::stoi(key);
            std::string dummy1, dummy2;
            std::string base_str, value_str;
            file >> dummy1 >> base_str >> dummy2 >> value_str;
            int base = std::stoi(base_str);
            ll y = decode_base(value_str, base);
            points[x].x = x;
            points[x].y = y;
        }
    }
    file.close();

    int count = 0;
    Point temp[MAX_POINTS];
    for (int i = 0; i < MAX_POINTS; ++i) {
        if (points[i].x != 0 || points[i].y != 0) {
            temp[count++] = points[i];
            if (count == k) break;
        }
    }
    for (int i = 0; i < k; ++i)
        points[i] = temp[i];

    return k;
}

int main() {
    Point points1[MAX_POINTS] = {};
    Point points2[MAX_POINTS] = {};
    int k1 = 0, k2 = 0;

    read_testcase("testcase1.txt", points1, k1);
    read_testcase("testcase2.txt", points2, k2);

    ll secret1 = lagrange(points1, k1);
    ll secret2 = lagrange(points2, k2);

    std::cout << secret1 << std::endl;
    std::cout << secret2 << std::endl;

    return 0;
}
