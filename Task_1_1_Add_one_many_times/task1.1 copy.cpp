#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Функция для вычисления НОД (наибольшего общего делителя)
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


int main() {
    // Оптимизация ввода/вывода для ускорения работы программы
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int k;
    cin >> k;
    
    // Читаем начальное состояние счетчика
    vector<int> bits(k);
    for (int i = 0; i < k; i++) {
        cin >> bits[i];
    }
    
    long long n;
    cin >> n;
    
    // Преобразуем начальное состояние в число
    long long start = 0;
    for (int i = 0; i < k; i++) {
        if (bits[i] == 1) {
            start |= (1LL << (k - 1 - i));
        }
    }
    
    long long max_changed = 0;
    long long total_changed = 0;
    
    // Ключевая оптимизация: счетчик имеет только k бит
    // Максимальное количество разных состояний = 2^k
    // Но по условию переполнения не будет, значит n может быть большим,
    // но мы обработаем максимум (2^k - start) значений
    
    long long max_states = (1LL << k) - start;
    long long actual_n = min(n, max_states);
    
    // Оптимизированный цикл: простой и быстрый
    for (long long x = start; x < start + actual_n; x++) {
        if (x >= (1LL << k)) break;
        
        // Быстрый подсчет младших единиц
        int trailing = 0;
        // Проверяем младшие биты до первого нуля
        for (int j = 0; j < k && j < 64; j++) {
            if (x & (1LL << j)) {
                trailing++;
            } else {
                break;
            }
        }
        
        int changed = trailing + 1;
        
        // Обновление максимума
        if (changed > max_changed) {
            max_changed = changed;
        }
        
        total_changed += changed;
    }
    
    // Если n > actual_n, значит остальные значения выходят за пределы k бит
    // Но по условию переполнения не будет, так что это не должно происходить
    // Однако на всякий случай: если n очень большое, остальные числа не валидны
    
    // Выводим максимальное количество измененных бит
    cout << max_changed << endl;
    
    // Вычисляем среднее как несократимую дробь
    long long numerator = total_changed;
    long long denominator = n;
    long long common_divisor = gcd(numerator, denominator);
    
    cout << numerator / common_divisor << "/" << denominator / common_divisor << endl;
    
    return 0;
}