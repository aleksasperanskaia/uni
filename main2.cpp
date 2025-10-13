#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool isLinear(const std::vector<int>& f, int n) {
    int size = 1 << n;

    // Для линейной функции без свободного члена f(0) = 0
    if (f[0] != 0) return false;

    // Коэффициенты a[i] = f(единичный вектор i)
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i)
        a[i] = f[1 << i];

    // Проверяем, что для всех x функция совпадает с суммой по модулю 2
    for (int x = 0; x < size; ++x) {
        int predicted = 0;
        for (int i = 0; i < n; ++i)
            if ((x >> i) & 1) predicted ^= a[i];
        if (predicted != f[x]) return false;
    }

    return true;
}

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    int k, n;
    std::string s;
    if (!(fin >> k >> n >> s)) return 0;  // Если чтение не удалось — выход

    // Проверяем корректность входных данных
    if (k != 2 || s.size() != (1u << n)) {
        fout << 0;
        return 0;
    }

    // Преобразуем строку в вектор 0 и 1
    std::vector<int> f;
    for (char c : s) {
        if (c != '0' && c != '1') { fout << 0; return 0; }
        f.push_back(c - '0');
    }

    fout << (isLinear(f, n) ? 1 : 0);
    return 0;
}
