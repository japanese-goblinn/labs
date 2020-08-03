//
//  main.c
//  task1
//
//  Created by Кирилл Горбачёнок on 18.02.2018.
//  Copyright © 2018 Кирилл Горбачёнок. All rights reserved.
//

#include <stdio.h>
#include <math.h>

void triangle(int, int, int);
	
int main(int argc, const char * argv[]) {
    int a, b, c;
    printf("Введит стороны треугольника: ");
    scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &c);
    if (a + b > c && a + c > b && b + c > a) {
        triangle(a, b, c);
    }
    else {
        printf("Такого треугольника не может существовать\n");
    }
    return 0;
}

void triangle(int a, int b, int c) {
    if ((a == b && a != c && b != c ) || (a == c && a != b && c != b) || (b == c && b != a && c != a)) {
        printf("Ваш треугольник равнобедренный\n");
    }
    else if (a == b && a == c && b == c) {
        printf("Ваш треугольник равносторонний\n");
    }
    else if ((pow(a, 2) + pow(b, 2) == pow(c, 2)) || (pow(a, 2) + pow(c, 2) == pow(b, 2)) || (pow(b, 2) + pow(c, 2) == pow(a, 2))) {
        printf("Ваш треугольник прямоугольный\n");
    }
    else {
        printf("Ваш треугольник общего вида\n");
    }
}
