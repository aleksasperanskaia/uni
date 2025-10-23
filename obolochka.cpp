#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: program input.txt output.txt\n";
        return 1;
    }

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    int k, n;
    string values;

    fin >> k >> n >> values;

    int total = 1;
    for (int i = 0; i < n; i++) total *= k;

    // Проверим, что длина таблицы корректна
    if ((int)values.size() != total) {
        cerr << "Incorrect input length\n";
        return 1;
    }

    // Для простоты — выделим все одноместные функции f(x_i)
    set<string> one_place_funcs;

    for (int var = 0; var < n; var++) {
        string func;
        for (int x = 0; x < k; x++) {
            // усредним по всем остальным переменным
            // чтобы получить зависимость только от var
            // (очень грубое приближение, но понятное)
            map<char, int> cnt;
            for (int mask = 0; mask < total; mask++) {
                int tmp = mask;
                int cur_val = tmp / (int)pow(k, var) % k;
                if (cur_val == x)
                    cnt[values[mask]]++;
            }
            // выбираем самое частое значение
            char best = '0';
            int bestc = -1;
            for (auto &p : cnt)
                if (p.second > bestc)
                    best = p.first, bestc = p.second;
            func.push_back(best);
        }
        one_place_funcs.insert(func);
    }

    // Добавим селекторные функции (0,1,2,...)
    for (int i = 0; i < k; i++) {
        string sel;
        for (int j = 0; j < k; j++)
            sel.push_back('0' + (i == j ? 1 : 0));
        one_place_funcs.insert(sel);
    }

    // Выводим лексикографически
    for (auto &f : one_place_funcs)
        fout << f << "\n";

    return 0;
}
