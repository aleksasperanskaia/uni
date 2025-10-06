#include <iostream>
#include <fstream>
#include <vector>

bool isLinear(const std::vector<int>& f, int n) {
    int size = 1 << n;

    if (f[0] != 0) return false;

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i)
        a[i] = f[1 << i];

    for (int x = 0; x < size; ++x) {
        int predicted = 0;
        for (int i = 0; i < n; ++i)
            if ((x >> i) & 1) predicted ^= a[i];

        if (predicted != f[x])
            return false;
    }

    return true;
}

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    int k, n;
    std::string s;
    fin >> k >> n >> s;
    fin.close();

    bool result = false;

    if (k == 2) {
        int size = 1 << n;
        if ((int)s.size() == size) {
            std::vector<int> f(size);
            bool valid_data = true;
            for (int i = 0; i < size; ++i) {
                if (s[i] != '0' && s[i] != '1') {
                    valid_data = false;
                    break;
                }
                f[i] = s[i] - '0';
            }

            if (valid_data) {
                result = isLinear(f, n);
            }
        }
    }

    fout << (result ? 1 : 0);
    fout.close();
    return 0;
}
