//
//  main.c
//  task2_2
//
//  Created by Кирилл Горбачёнок on 19.02.2018.
//  Copyright © 2018 Кирилл Горбачёнок. All rights reserved.
//

#include <stdio.h>
#include <math.h>

/*sinx=x−x3 +x5 −...+(−1)n−1 x2n−1 +... 3! 5! (2n−1)!
Численно убедиться в справедливости равенства, для чего для заданного значения
х вычислить его левую часть и разложение, стоящее в правой части.
При каком исследуемое выражение отличается от sin x менее, чем на заданную погрешность ε?
Испытать разложение на сходимость при разных значениях х.
*/

double recurscalc(int n, double x) {
    if (n == 1) {
        return x;
    }
    double result = 0;
    result += (pow(-1, n - 1) * pow(x, 2*n - 1)) / fact(2*n - 1);
    result += recurscalc(n-1, x);
    return result;
    
}

double forcalc(int n, double x) {
    double result = 0;
    for (int i = 1; i <= n; i++) {
        result += (pow(-1, i - 1) * pow(x, 2*i - 1)) / fact(2*i - 1);
    }
    return result;
}

int fact(int n) {
    if (n == 1) {
        return 1;
    }
    int result = n * fact(n - 1);
    return result;
}

int main(int argc, const char * argv[]) {
    double x = 0;
    int n = 1;
    double e;
    printf("Введите x: ");
    scanf("%lf", &x);
    printf("Введит эпсилон: ");
    scanf("%lf", &e);
    double left = sin(x);
    double right;
    int chose = 1;
    int loop = 1;
    while (loop) {
        printf("Выбор способа подсчета:\n");
        printf("1) Рекурсивно;\n");
        printf("2) Итерационно;\n");
        printf("Выбор: ");
        scanf("%d", &chose);
        switch (chose) {
            case 1:
                for (int i = 1; i <= n; i++) {
                    right = recurscalc(i, x);
                    if (right - left < e) {
                        printf("Результат вычисления слева: %lf\n", left);
                        printf("Результат вычисления справа: %lf\n", right);
                        printf("При n = %d выржение отличается менее чем на заданную погрешность;\n", n);
                        break;
                    }
                    n++;
                }
                loop = 0;
                break;
            case 2:
                for (int i = 1; i <= n; i++) {
                    right = forcalc(n, x);
                    if (right - left < e) {
                        printf("Результат вычисления слева: %lf\n", left);
                        printf("Результат вычисления справа: %lf\n", right);
                        printf("При n = %d выржение отличается менее чем на заданную погрешность;\n", n);
                        break;
                    }
                    n++;
                }
               
                loop = 0;
                break;
            default:
                printf("\nОШИБКА ВВОДА\n");
                break;
        }
    }
    return 0;
}
