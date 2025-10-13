#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    int k, n;
    std::string s;
    if (!(fin >> k >> n >> s) || k != 2 || s.size() != (1u << n)) {
        fout << 0;
        return 0;
    }

    std::vector<int> f;
    for (char c : s) {
        if (c != '0' && c != '1') { fout << 0; return 0; }
        f.push_back(c - '0');
    }

    if (f[0] != 0) { fout << 0; return 0; }  // Линейная функция должна иметь f(0)=0

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = f[1 << i];

    for (int x = 0; x < (1 << n); ++x) {
        int predicted = 0;
        for (int i = 0; i < n; ++i)
            if ((x >> i) & 1) predicted ^= a[i];
        if (predicted != f[x]) { fout << 0; return 0; }
    }

    fout << 1;
    return 0;
}
