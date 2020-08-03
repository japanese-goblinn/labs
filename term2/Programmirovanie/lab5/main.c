//
//  main.c
//  lab5
//
//  Created by Кирилл Горбачёнок on 30.04.2018.
//  Copyright © 2018 Кирилл Горбачёнок. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

/*
Даны две упорядоченные очереди. Разработать функцию, которая объединяет их в один упорядоченный
двунаправленный список.
*/

struct List
{
    struct List *next;
    struct List *prev;
    int value;
};

struct Queue
{
    struct Queue *prev;
    int value;
};

struct Queue *firstBackQue = NULL, *firstFrontQue = NULL;
struct Queue *secondBackQue = NULL, *secondFrontQue = NULL;
struct List *head = NULL, *tail = NULL;


void ShowList()
{
    struct List *tmp = head;
    while (tmp)
    {
        printf("%d\n", tmp->value);
        tmp = tmp->next;
    }
}


int Pop()
{
    int toReturn = firstFrontQue->value;
    firstFrontQue = firstFrontQue->prev;
    return toReturn;
}

void Push(int value)
{
    if (firstBackQue == NULL)
    {
        firstBackQue = (struct Queue *)malloc(sizeof(struct Queue));
        firstFrontQue = firstBackQue;
        firstBackQue->value = value;
        return;
    }
    firstBackQue->prev = (struct Queue *)malloc(sizeof(struct Queue));
    firstBackQue = firstBackQue->prev;
    firstBackQue->prev = NULL;
    firstBackQue->value = value;
}

int Pop2()
{
    int toReturn = secondFrontQue->value;
    secondFrontQue = secondFrontQue->prev;
    return toReturn;
}

void Push2(int value)
{
    if (secondBackQue == NULL)
    {
        secondBackQue = (struct Queue *)malloc(sizeof(struct Queue));
        secondFrontQue = secondBackQue;
        secondBackQue->value = value;
        return;
    }
    secondBackQue->prev = (struct Queue *)malloc(sizeof(struct Queue));
    secondBackQue = secondBackQue->prev;
    secondBackQue->prev = NULL;
    secondBackQue->value = value;
}

void Association()
{
    head = (struct List *)malloc(sizeof(struct List));
    struct List *prev = NULL;
    while (firstFrontQue)
    {
        if (tail == NULL)
        {
            tail = head;
            tail->value = Pop();
            tail->next = NULL;
            tail->prev = NULL;
            prev = tail;
        }
        else
        {
            tail->next = (struct List *)malloc(sizeof(struct List));
            tail = tail->next;
            tail->prev = prev;
            tail->value = Pop();
            tail->next = NULL;
            prev = tail;
        }
    }
    while (secondFrontQue)
    {
        if (tail == NULL)
        {
            tail = head;
            tail->value = Pop2();
            tail->next = NULL;
            tail->prev = NULL;
            prev = tail;
        }
        else
        {
            tail->next = (struct List *)malloc(sizeof(struct List));
            tail = tail->next;
            tail->prev = prev;
            tail->value = Pop2();
            tail->next = NULL;
            prev = tail;
        }
    }
}

int main(int argc, const char * argv[])
{
    for (int i = 0; i < 10; i++)
    {
        Push(i);
        Push2(i + i);
    }
    Association();
    ShowList();
    return 0;
}
