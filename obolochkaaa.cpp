#include <bits/stdc++.h>
using namespace std;

// перевод символа (0-9, A-Z) -> число
int ch2v(char c){
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'A' && c <= 'Z') return 10 + (c - 'A');
    if(c >= 'a' && c <= 'z') return 10 + (c - 'a'); // на случай маленьких
    return 0;
}
// обратный перевод числа -> символ
char v2ch(int v){
    if(v < 10) return char('0' + v);
    return char('A' + (v - 10));
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int k, n;
    if(!(cin >> k >> n)) return 0;
    string s;
    cin >> s;
    // ожидаем s.length() == k^n
    long long total = 1;
    for(int i=0;i<n;i++){
        total *= k;
        if(total > (1LL<<60)) break;
    }
    if((long long)s.size() != total){
        // нечёткий ввод — но попытаемся продолжить, либо выходим
        // в задачах контеста вход корректен
    }
    // f stored in vector in lexicographic order: index = sum_{i=0..n-1} a[i]*k^{n-1-i}
    vector<int> f;
    f.reserve(s.size());
    for(char c : s) f.push_back(ch2v(c));

    // Precompute powers of k
    vector<long long> powk(n+1);
    powk[0] = 1;
    for(int i=1;i<=n;i++) powk[i] = powk[i-1] * k;

    // Функция для доступа: given tuple a[0..n-1] (0..k-1), возвращаем f[a]
    auto getF = [&](const vector<int>& a)->int{
        long long idx = 0;
        for(int i=0;i<n;i++){
            idx = idx * k + a[i];
        }
        return f[(size_t)idx];
    };

    // Собираем начальные унарные функции:
    // все варианты: выбираем позицию pos где стоит x, остальные фиксируем константами.
    // Также учтём константные функции (когда нет x).
    // Представим унарную функцию как string длины k (символы 0..k-1 -> char для удобства).
    auto vec_to_key = [&](const vector<int>& arr)->string{
        string t;
        t.resize(k);
        for(int i=0;i<k;i++) t[i] = v2ch(arr[i]);
        return t;
    };

    unordered_set<string> seen;
    vector<string> list_funcs; list_funcs.reserve(1024);

    // 1) все случаи: выбрали пози pos as variable
    // переберём все комбинации констант для остальных n-1 позиций
    if(n == 0){
        // нулевая переменная — функция константа
        vector<int> val(k, f.empty()?0:f[0]);
        string key = vec_to_key(val);
        seen.insert(key);
        list_funcs.push_back(key);
    } else {
        // Для генерации всех (k^(n-1)) комбинаций используем счетчик
        for(int pos = 0; pos < n; ++pos){
            int other = n - 1;
            long long combos = powk[other];
            for(long long comb = 0; comb < combos; ++comb){
                // распаковать comb в base-k представление длины other
                vector<int> tuple(n);
                long long x = comb;
                for(int i = n-1; i >= 0; --i){
                    if(i == pos) continue;
                    tuple[i] = x % k;
                    x /= k;
                }
                // сформируем унарную функцию g(t) = f(tuple with tuple[pos] = t)
                vector<int> g(k);
                for(int t = 0; t < k; ++t){
                    tuple[pos] = t;
                    g[t] = getF(tuple);
                }
                string key = vec_to_key(g);
                if(!seen.count(key)){
                    seen.insert(key);
                    list_funcs.push_back(key);
                }
            }
        }
        // 2) константные функции (все аргументы фиксированы): это частный случай comb с "нет x"
        long long combosAll = powk[n];
        for(long long comb = 0; comb < combosAll; ++comb){
            vector<int> tuple(n);
            long long x = comb;
            for(int i = n-1; i >= 0; --i){
                tuple[i] = x % k;
                x /= k;
            }
            int val = getF(tuple);
            vector<int> g(k, val);
            string key = vec_to_key(g);
            if(!seen.count(key)){
                seen.insert(key);
                list_funcs.push_back(key);
            }
        }
        // 3) по определению проекции (id) порождаются любой функцией -> добавим явный id
        {
            vector<int> idf(k);
            for(int t=0;t<k;t++) idf[t] = t;
            string key = vec_to_key(idf);
            if(!seen.count(key)){
                seen.insert(key);
                list_funcs.push_back(key);
            }
        }
    }

    // Теперь делаем замыкание по композиции: если a,b в множестве, то a∘b добавляем.
    // Поддерживаем список list_funcs и set seen; при добавлении нового компонуем с уже имеющимися в обе стороны.
    for(size_t ptr = 0; ptr < list_funcs.size(); ++ptr){
        const string &newf = list_funcs[ptr];
        // компонуем newf с уже имеющимися (включая сам newf и предыдущие)
        // для экономии: проходим по всем существующим индексам j (0..list_funcs.size()-1)
        // но list_funcs растёт, поэтому аккуратно: для j от 0..list_funcs.size()-1
        for(size_t j = 0; j < list_funcs.size(); ++j){
            // compose A = newf ∘ list_funcs[j]
            // тут list_funcs[j] = h, newf = g -> (g ∘ h)(x) = g( h(x) )
            string comp(k, '0');
            const string &h = list_funcs[j];
            for(int x = 0; x < k; ++x){
                char hx = h[x];
                // hx - символ '0'..'9','A'..
                // индекс into g: need convert hx to index 0..k-1
                int idx;
                if(hx >= '0' && hx <= '9') idx = hx - '0';
                else idx = 10 + (hx - 'A');
                comp[x] = newf[idx];
            }
            if(!seen.count(comp)){
                seen.insert(comp);
                list_funcs.push_back(comp);
            }
            // compose B = list_funcs[j] ∘ newf
            string comp2(k, '0');
            for(int x = 0; x < k; ++x){
                char gx = newf[x];
                int idx;
                if(gx >= '0' && gx <= '9') idx = gx - '0';
                else idx = 10 + (gx - 'A');
                comp2[x] = h[idx];
            }
            if(!seen.count(comp2)){
                seen.insert(comp2);
                list_funcs.push_back(comp2);
            }
        }
        // цикл ptr увеличивается, пока не переберём все новые
    }

    // Сортируем лексикографически строки и выводим по одной на строку (без количества)
    sort(list_funcs.begin(), list_funcs.end());
    for(const string &r : list_funcs){
        cout << r << '\n';
    }
    return 0;
}
