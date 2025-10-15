#include <iostream>
#include <fstream>
#include <vector>

bool isLinear(const std::vector<int>& f, int n) {
    int size = 1 << n;
    
    // Пробуем две возможности: с константой 0 и с константой 1
    for (int constant = 0; constant <= 1; ++constant) {
        std::vector<int> a(n);
        bool valid = true;
        
        // Находим коэффициенты при переменных
        for (int i = 0; i < n; ++i) {
            // f(e_i) = constant ⊕ a_i, поэтому a_i = f(e_i) ⊕ constant
            a[i] = f[1 << i] ^ constant;
        }
        
        // Проверяем все значения
        for (int x = 0; x < size; ++x) {
            int predicted = constant;  // начинаем с константы
            for (int i = 0; i < n; ++i) {
                if ((x >> i) & 1) {
                    predicted ^= a[i];
                }
            }
            
            if (predicted != f[x]) {
                valid = false;
                break;
            }
        }
        
        if (valid) {
            return true;
        }
    }
    
    return false;
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
