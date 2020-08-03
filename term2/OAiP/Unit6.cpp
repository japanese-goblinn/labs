//---------------------------------------------------------------------------


#pragma hdrstop
#include "vcl.h"
#include "Unit6.h"
#include <iostream>
#include <ctime>

bool Queue::IsEmpty() {
	if (head == NULL) {
	   return true;
	}
	else {
		return false;
    }
}

void Queue::Show(TListBox *ListBox1) {
	Data *queue = head;
	while (queue)
	{
		ListBox1->Items->Add(queue->num);
		queue = queue->next;
	}
}

void Queue::Push(int addnum) {
	Data *tmp = new Data;
	tmp->num = addnum;
	if (head == NULL) {
	   head = tmp;
	   tail = tmp;
	   tail->next = NULL;
	   tail->prev = NULL;
	}
	else {
		tail->next = tmp;
		tmp->prev = tail;
		tail = tail->next;
		tail->next = NULL;
	}
}

int Queue::Pop() {
	int num = head->num;
	head = head->next;
	return num;
}

void Queue::Clear() {
	while (head) {
		Pop();
		if (head->next->next == NULL) {
			head = NULL;
			return;
		}
		else {
			head = NULL;
			return;
        }
		head = head->next;
	}

}

void IntNums::GenRandInts(int randm) {
	srand(time(NULL));
	for (int i = 0; i < randm; i++) {
		int a =  rand() % 101 + (-50);
		Push(a);
	}
}

void IntNums::DivideOnTwo(TListBox *ListBox2, TListBox *ListBox3) {
	while (head) {
		if (head->num >= 0) {
		   if (headForPos == NULL) {
			  headForPos = head;
			  tailForPos = head;
			  head = head->next;
			  tailForPos->next = NULL;
			  tailForPos->prev = NULL;
		   }
		   else {
			  tailForPos->next = head;
			  head = head->next;
			  Data *tmp = tailForPos;
			  tailForPos = tailForPos->next;
			  tailForPos->prev = tmp;
		   }
		}
		else if (head->num < 0) {
			if (headForNeg == NULL) {
			  headForNeg = head;
			  tailForNeg = head;
			  head = head->next;
			  tailForNeg->next = NULL;
			  tailForNeg->prev = NULL;
			}
			else {
			  tailForNeg->next = head;
			  head = head->next;
			  Data *tmp = tailForNeg;
			  tailForNeg = tailForNeg->next;
			  tailForNeg->prev = tmp;
			}
		}
	}
	Data *top1 = headForPos;
	while (top1 && top1->num >= 0) {
		ListBox2->Items->Add(top1->num);
		top1 = top1->next;
	}
	Data *top2 = headForNeg;
	while (top2 && top2->num < 0) {
		ListBox3->Items->Add(top2->num);
		top2 = top2->next;
	}
}









//---------------------------------------------------------------------------

#pragma package(smart_init)
