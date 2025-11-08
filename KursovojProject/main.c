
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>

// =========================== ФУНКЦИЯ f(x) ==============================
double f(double x) {
    if (x < 0) {
        double denom = log(3 + fabs(x));
        if (denom == 0.0) {
            printf("Ошибка: ln(3 + |x|) = 0 не определено для x = %.3lf\n", x);
            exit(1);
        }
        return sin(x * x) / denom;
    }
    else if (x < 5) {
        return atan(x) * exp(cos(x)) / cbrt(1 + x * x);
    }
    else {
        double sum = 0.0;
        for (int n = 0; n <= 7; n++) {
            double denom = tgamma(n + 1) * log(n + 4);
            if (denom == 0.0) {
                printf("Ошибка: log(n + 4) = 0 при n = %d\n", n);
                exit(1);
            }
            sum += pow(-1, n) * pow(x, n) / denom;
        }
        return sum;
    }
}

// ======================== ПРОИЗВОДНАЯ ЧЕРЕЗ ЧИСЛО =====================
double derivative(double x) {
    double h = 1e-6;
    return (f(x + h) - f(x - h)) / (2 * h);
}

// =========================== ЭКСТРЕМУМЫ ================================
void find_extrema(double a, double b, int steps) {
    double x_min = a, x_max = a;
    double f_min = f(a), f_max = f(a);
    double step = (b - a) / steps;

    for (int i = 0; i <= steps; i++) {
        double x = a + i * step;
        double y = f(x);
        if (y < f_min) { f_min = y; x_min = x; }
        if (y > f_max) { f_max = y; x_max = x; }
    }

    printf("Минимум: f(%lf) = %lf\n", x_min, f_min);
    printf("Максимум: f(%lf) = %lf\n", x_max, f_max);
}

// ========================== ТАБЛИЦА ЗНАЧЕНИЙ ==========================
void table(double a, double b, double step) {
    printf("\n=============================================\n");
    printf("|     x      |         f(x)                 |\n");
    printf("=============================================\n");

    for (double x = a; x <= b + step / 2; x += step)
        printf("| %10.4lf | %25.8lf    |\n", x, f(x));

    printf("=============================================\n");
}

// ========================== ПОИСК x ДЛЯ f(x) ≈ Y =====================
double find_x(double a, double b, double Y) {
    double mid, fm;
    int max_iter = 100;
    for (int i = 0; i < max_iter; i++) {
        mid = (a + b) / 2;
        fm = f(mid);
        if (fabs(fm - Y) < 1e-6) break;
        if ((f(a) - Y) * (fm - Y) < 0)
            b = mid;
        else
            a = mid;
    }
    return mid;
}

// =========================== ПРОВЕРКА ВВОДА ===========================
double input_double(const char* msg) {
    double x;
    printf("%s", msg);
    while (scanf("%lf", &x) != 1) {
        printf("Ошибка ввода. Повторите: ");
        while (getchar() != '\n'); // очистка буфера
    }
    return x;
}

// ============================== МЕНЮ ==================================
int main() {
    setlocale(LC_CTYPE, "RUS");
    int choice;
    printf("=== Консольное меню ===\n");

    while (1) {
        printf("\nВыберите операцию:\n");
        printf("1 - Значение f(x) в точке\n");
        printf("2 - Таблица значений на интервале\n");
        printf("3 - Поиск минимума и максимума на отрезке\n");
        printf("4 - Поиск x, где f(x) ≈ Y\n");
        printf("5 - Производная f'(x) в точке\n");
        printf("0 - Выход\n> ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода.\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice == 0) break;

        double x, a, b, step, Y;

        switch (choice) {
        case 1:
            x = input_double("Введите x: ");
            printf("f(%.6lf) = %.8lf\n", x, f(x));
            break;

        case 2:
            a = input_double("Введите начало интервала a: ");
            b = input_double("Введите конец интервала b: ");
            step = input_double("Введите шаг: ");
            if (step <= 0 || a >= b) {
                printf("Ошибка: некорректный интервал или шаг.\n");
                break;
            }
            table(a, b, step);
            break;

        case 3:
            a = input_double("Введите начало отрезка a: ");
            b = input_double("Введите конец отрезка b: ");
            if (a >= b) {

                    printf("Ошибка: a должно быть меньше b.\n");
                break;
            }
            find_extrema(a, b, 1000);
            break;

        case 4:
            a = input_double("Введите начало интервала a: ");
            b = input_double("Введите конец интервала b: ");
            Y = input_double("Введите значение Y: ");
            printf("x ≈ %.6lf, f(x) = %.8lf\n", find_x(a, b, Y), f(find_x(a, b, Y)));
            break;

        case 5:
            x = input_double("Введите x: ");
            printf("f'(%.6lf) ≈ %.8lf\n", x, derivative(x));
            break;

        default:
            printf("Неверный выбор. Повторите.\n");
        }
    }

    printf("Выход из программы.\n");
    return 0;
}