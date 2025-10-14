#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    int k, n;
    std::string s;
    fin >> k >> n >> s;

    std::vector<int> f;
    for (char c : s)
        f.push_back(c - '0');

    int c0 = f[0];           // свободный член (константа)
    std::vector<int> a(n);

    for (int i = 0; i < n; ++i)
        a[i] = f[1 << i] ^ c0;  // коэффициенты при x_i

    for (int x = 0; x < (1 << n); ++x) {
        int predicted = c0;
        for (int i = 0; i < n; ++i)
            if ((x >> i) & 1)
                predicted ^= a[i];
        if (predicted != f[x]) {
            fout << 0;
            return 0;
        }
    }

    fout << 1;
    return 0;
}
