#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

// Функция для генерации переменной с повторяющимися значениями
vector<int> make_repeating_var(int radix, int length) {
    vector<int> var;
    for (int i = 0; i < length; ++i) {
        var.push_back(i % radix);
    }
    return var;
}

// Функция для генерации всех возможных комбинаций источников
vector<vector<vector<int>>> make_all_combinations(const vector<int>& seed, const vector<vector<int>>& derived_list) {
    vector<vector<vector<int>>> combos;

    // Список всех источников (seed + ранее порождённые функции)
    vector<vector<int>> all_sources = {seed};
    all_sources.insert(all_sources.end(), derived_list.begin(), derived_list.end());

    int total_sources = all_sources.size();

    // Генерация всех пар источников (каждая позиция может брать значение из любого источника)
    for (int i = 0; i < total_sources; ++i) {
        for (int j = 0; j < total_sources; ++j) {
            vector<vector<int>> pair = {all_sources[i], all_sources[j]};
            combos.push_back(pair);
        }
    }

    return combos;
}

// Функция вычисления порождённой функции по таблице baseFunc и значениям двух аргументов
vector<int> compute_derived_function(const vector<int>& baseFunc,
                                     const vector<int>& a_vals,
                                     const vector<int>& b_vals,
                                     int radix,
                                     set<vector<int>>& unique_set) {
    vector<int> resultVec;

    for (int i = 0; i < (int)a_vals.size(); ++i) {
        int a = a_vals[i];
        int b = b_vals[i];

        // Вычисление индекса для k-значной логики
        int index = a * radix + b;
        resultVec.push_back(baseFunc[index]);
    }

    if (unique_set.find(resultVec) == unique_set.end()) {
        unique_set.insert(resultVec);
        return resultVec;
    } else {
        return {};
    }
}

int main() {
    ifstream inFile("input.txt");
    ofstream outFile("output.txt");

    if (!inFile.is_open()) {
        cerr << "Error: Could not open input file." << endl;
        return 1;
    }

    if (!outFile.is_open()) {
        cerr << "Error: Could not open output file." << endl;
        return 1;
    }

    int radix;           // Основа логики (k)
    int arity;           // Количество переменных (n)
    inFile >> radix >> arity;

    // Чтение строки значений функции
    string funcStr;
    inFile >> funcStr;

    // Преобразование строки в вектор значений функции
    vector<int> baseFunction;
    for (char ch : funcStr) {
        baseFunction.push_back(ch - '0');
    }

    if (baseFunction.size() != (size_t)pow(radix, arity)) {
        cerr << "Error: Function values size does not match the number of variables for k-ary logic." << endl;
        return 1;
    }

    // Генерация изначального вектора переменной seedVar (для первой позиции)
    int combosCount = (int)pow(radix, arity - 1);
    vector<int> seedVar = make_repeating_var(radix, combosCount);

    // Хранилище всех уникальных функций
    set<vector<int>> unique_set;

    // Вектор для хранения всех порождённых функций
    vector<vector<int>> derivedFunctions;

    // Первая итерация: генерируем комбинации только для seedVar и seedVar (или второй отсутствующей функции)
    vector<vector<vector<int>>> combos = make_all_combinations(seedVar, {});

    // Цикл генерации новых функций пока появляются новые
    bool hasNew = true;
    while (hasNew) {
        hasNew = false;
        vector<vector<int>> newDerived; // временное хранение новых уникальных функций

        // Обработка каждой комбинации (пары источников)
        for (const auto& pair : combos) {
            // 1) Оригинальный порядок (A, B)
            vector<int> derived = compute_derived_function(baseFunction, pair[0], pair[1], radix, unique_set);
            if (!derived.empty()) {
                newDerived.push_back(derived);
                hasNew = true;
            }

            // 2) Автоматическая перестановка: (B, A)
            //    Пробуем обратный порядок — это и есть "переставить автоматически"
            vector<int> derived_swapped = compute_derived_function(baseFunction, pair[1], pair[0], radix, unique_set);
            if (!derived_swapped.empty()) {
                newDerived.push_back(derived_swapped);
                hasNew = true;
            }
        }

        // Добавляем новые функции к списку порождённых
        for (const auto& fvec : newDerived) {
            derivedFunctions.push_back(fvec);
        }

        // Перегенерация всех комбинаций с учётом новых порождённых функций
        combos = make_all_combinations(seedVar, derivedFunctions);
    }

    // Итоговый вывод всех уникальных функций в файл
    for (const auto& ufunc : unique_set) {
        outFile << radix << " " << 1 << " ";
        for (int v : ufunc) {
            outFile << v;
        }
        outFile << endl;
    }

    inFile.close();
    outFile.close();

    return 0;
}
