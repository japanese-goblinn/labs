//
//  main.cpp
//  lab6
//
//  Created by Кирилл Горбачёнок on 30.04.2018.
//  Copyright © 2018 Кирилл Горбачёнок. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <math.h>

struct Customer
{
    int val;
    unsigned int height;
    Customer *right = NULL;
    Customer *left = NULL;
};

struct Queue
{
    Customer *root = NULL;
} que;

unsigned int Height(Customer *current) {
    return current ? current->height : 0;
}

int balanceFactor(Customer *current) {
    return Height(current->right) - Height(current->left);
}

void FixHeigth(Customer *current) {
    if (current == NULL)
    {
        return;
    }
    unsigned int heightLeft = Height(current->left);
    unsigned int heightRight = Height(current->right);
    current->height = (heightLeft > heightRight ? heightLeft : heightRight) + 1;
}

Customer* RotateRight(Customer* p) {
    Customer* q = p->left;
    p->left = q->right;
    q->right = p;
    FixHeigth(p);
    FixHeigth(q);
    return q;
}

Customer* RotateLeft(Customer* q)
{
    Customer* p = q->right;
    q->right = p->left;
    p->left = q;
    FixHeigth(q);
    FixHeigth(p);
    return p;
}

Customer *Balance(Customer *current) {
    FixHeigth(current);
    if (balanceFactor(current) == 2)
    {
        if (balanceFactor(current->right) < 0)
        {
            current->right = RotateRight(current->right);
        }
        return RotateLeft(current);
    }
    if (balanceFactor(current) == -2)
    {
        if (balanceFactor(current->left) > 0)
        {
            current->left = RotateLeft(current->left);
        }
        return RotateRight(current);
    }
    return current;
}

Customer* Add(Customer* p, int k) {
    if (!p) {
        p = (Customer*)malloc(sizeof(Customer));
        p->right = NULL;
        p->left = NULL;
        p->val = k;
        p->height = 1;
        return p;
    }
    if (k<p->val)
        p->left = Add(p->left, k);
    else
        p->right = Add(p->right, k);
    return Balance(p);
}

int _print_t(Customer *tree, int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    int width = 5;
    if (!tree) return 0;
    sprintf(b, "(%03d)", tree->val);
    int left = _print_t(tree->left, 1, offset, depth + 1, s);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];
    if (depth && is_left) {
        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width / 2 + i] = '-';
        s[2 * depth - 1][offset + left + width / 2] = '+';
        s[2 * depth - 1][offset + left + width + right + width / 2] = '+';
    }
    else if (depth && !is_left) {
        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width / 2 + i] = '-';
        s[2 * depth - 1][offset + left + width / 2] = '+';
        s[2 * depth - 1][offset - width / 2 - 1] = '+';
    }
    return left + width + right;
}

void print_t(Customer *tree)
{
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");
    _print_t(tree, 0, 0, 0, s);
    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}

int StrLen(char *string) {
    int count = 0;
    while (string[count] != NULL)
    {
        count++;
    }
    return count;
}

int StrToInt() {
    int i = 0, digit = 0;
    char stringDigit[1000];
    while (1)
    {
        scanf("%s", stringDigit);
        for (; i < 10 || stringDigit[i] != NULL; i++)
        {
            if ((stringDigit[i] > '9' || stringDigit[i] < '0') || StrLen(stringDigit) > 10)
            {
                printf("Not int or digit. Try again.\n");
                digit = 0;
                break;
            }
            else
            {
                digit += (stringDigit[i] - '0') * pow(10, StrLen(stringDigit) - i - 1);
            }
            if (i == StrLen(stringDigit) - 1)
            {
                return digit;
            }
        }
    }
}

int Found(Customer *p, int value)
{
    if (p)
    {
        if (p->val < value)
        {
            return Found(p->right, value);
        }
        else if (p->val > value)
        {
            return Found(p->left, value);
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    printf("START");
    while (1) {
        printf("MENU\n--------------------------\nSelect what you wanna do:\n 1.Add node's\n 2.Show tree\n 0. Exit\n");
        char selection[10000];
        scanf("%s", selection);
        int value;
        switch (selection[0])
        {
            case '1':
                printf("Enter val to add: ");
                value = StrToInt();
                if (Found(que.root, value) == false)
                {
                    que.root = Add(que.root, value);
                }
                while (value != 0)
                {
                    printf("Enter val to add: ");
                    value = StrToInt();
                    if (Found(que.root, value) == false)
                    {
                        que.root = Add(que.root, value);
                    }
                }
                value = NULL;
                break;
            case '2':
                print_t(que.root);
                break;
            case '0':
                return 0;
            default:
                printf("Error, enter 1, 2, 3 or 0: ");
                break;
        }
    }
    return 0;
}
