//
//  main.c
//  task1_3
//
//  Created by Кирилл Горбачёнок on 21.02.2018.
//  Copyright © 2018 Кирилл Горбачёнок. All rights reserved.
//

#include <stdio.h>

/*
 Матрицу a(m, n) заполнить следующим образом. Для заданных k и l элементу akl присвоить значение 1;
 элементам,окаймляющим его (соседним с ним по вертикали, горизонтали и диагонали) – значение 2; элементам
 следующего окаймления – значение 3 и так далее до заполнения всей матрицы.
 Примечание. Алгоритм не изменится, если координаты элемента (несуществующего) k и l находятся за пределами
 матрицы.
 */

int AIsZero(int arr[][10], int n, int n1) {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n1; j++)
        {
            if (arr[i][j] == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    int x = 3;
    int y = x++;
    printf("%d", y);
    printf("%d", x);
    y = --x;
    printf("%d", y);
    printf("%d", x);
//    const int n = 10, m = 10;
//    int a[n][m] = { 0 };
//    int k, l;
//    printf("Enter k and l for an array a(k,l) element:\n");
//    scanf("%d%d", &k, &l);
//    int globalPlus = 1;
//    if (k >= 0 && k < n && l < m && l >= 0)
//    {
//        a[k][l] = 1;
//    }
//    while (AIsZero(a, n, m))
//    {
//        for (int i = k + globalPlus, j = l + globalPlus; i >= k - globalPlus; i--)
//        {
//            if (i >= 0 && i < n && j < m && j >= 0)
//            {
//                a[i][j] = globalPlus + 1;
//            }
//        }
//        for (int i = k - globalPlus, j = l + globalPlus; j >= k - globalPlus; j--)
//        {
//            if (i >= 0 && i < n && j < m && j >= 0)
//            {
//                a[i][j] = globalPlus + 1;
//            }
//        }
//        for (int i = k - globalPlus, j = l - globalPlus; i <= k + globalPlus; i++)
//        {
//            if (i >= 0 && i < n && j < m && j >= 0)
//            {
//                a[i][j] = globalPlus + 1;
//            }
//        }
//        for (int i = k + globalPlus, j = l - globalPlus; j <= k + globalPlus; j++)
//        {
//            if (i >= 0 && i < n && j < m && j >= 0)
//            {
//                a[i][j] = globalPlus + 1;
//            }
//        }
//        globalPlus++;
//    }
//    for (int i = 0; i < n; i++)
//    {
//        for (int j = 0; j < m; j++)
//        {
//            printf("%d", a[i][j]);
//            printf(" ");
//        }
//        printf("\n");
//    }
    return 0;
}
