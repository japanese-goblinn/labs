//
//  main.c
//  task 2
//
//  Created by Кирилл Горбачёнок on 18.02.2018.
//  Copyright © 2018 Кирилл Горбачёнок. All rights reserved.
//

#include <stdio.h>
/*
 Дано четырехзначное число. Найти число, образованное перестановкой двух первых и двух последних цифр заданного числа.
 Например, из числа 4566 получается 6645, из числа 7304 – 473.
 */

int main(int argc, const char * argv[]) {
    int num;
    printf("Введите начальное число: ");
    scanf("%d", &num);
    int endnum = 0;
    endnum += (num % (int)pow(10,2)) * (int)pow(10,2);
    endnum += num / (int)pow(10,2);
    printf("Ваше новое число: %d \n", endnum);
    return 0;
}
