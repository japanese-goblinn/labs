//
//  main.c
//  lab4_2
//
//  Created by Кирилл Горбачёнок on 17.03.2018.
//  Copyright © 2018 Кирилл Горбачёнок. All rights reserved.
//

#include <stdio.h>

/*
В текстовом файле находится произвольный текст. Разработать программу проверки правильности расстановки скобок
(круглых, квадратных, фигурных). Критерий проверки: если встречается одна из закрывающих скобок, то последняя
открывающая должна быть такого же типа; количество скобок каждого типа должно совпадать. Между скобками допустима
запись любых символов.
*/


int main(int argc, const char * argv[]) {
    FILE *fp;
    char name[] = "textFor4lab.txt";
    if (fopen(name, "r") != NULL) {
        fp = fopen(name, "r");
        printf("congrats друг\n\n");
        char string[256] = "";
        while (!feof(fp)) {
            fgets(string, 256, fp);
            printf("%s", string);
        }
        printf(" ");
        char check[50] = "";
        int forC = 0;
        int forK = 0;
        int forF = 0;
        for (int i = 0, j = 0; string[i] != '\0'; i++) {
            if (string[i] == '[' || string[i] == '(' || string[i] == '{' || string[i] == ']' || string[i] == ')' || string[i] == '}') {
                check[j] = string[i];
                j++;
            }
        }
        for (int i = 0; check[i] != '\0'; i++) {
            if (check[i] == '[' || check[i] == ']') {
                forK++;
            }
            if (check[i] == '(' || check[i] == ')') {
                forC++;
            }
            if (check[i] == '{' || check[i] == '}') {
                forF++;
            }
        }
        int cnt = 0;
        for (int i = 0; check[i] != '\0'; i++) {
            if (check[i] == '[') {
                int j = i;
                while(check[j] != '\0') {
                    if (check[j] == ']') {
                        break;
                    }
                    if (check[j] == '}' || check[j] == ')') {
                        cnt++;
                        printf("\nBRAKES ARE NOT PUT RIGHT\n");
                        break;
                    }
                    j++;
                }
            }
            else if (check[i] == '(') {
                int j = i;
                while(check[j] != '\0') {
                    if (check[j] == ')') {
                        break;
                    }
                    if (check[j] == '}' || check[j] == ']') {
                        cnt++;
                        printf("\nBRAKES ARE PUT NOT RIGHT\n");
                        break;
                    }
                    j++;
                }
            }
            else if (check[i] == '{') {
                int j = i;
                while(check[j] != '\0') {
                    if (check[j] == '}') {
                        break;
                    }
                    if (check[j] == ']' || check[j] == ')') {
                        cnt++;
                        printf("\nBRAKES ARE PUT NOT RIGHT\n");
                        break;
                    }
                    j++;
                }
            }
        }
        if (!cnt) {
            if (forF != forK && forF != forC) {
                printf("\nBRAKES ARE PUT NOT RIGHT\n");
            }
            else {
                printf("\nBRAKES ARE PUT RIGHT\n");
            }
        }
    }
    else {
        printf("how could u друг?\n");
    }
    int ifClose = fclose(fp);
    if (!ifClose) {
        printf("\nfile r closed\n");
    }
    else {
        printf("file r not closed\n");
    }
    return 0;
}
