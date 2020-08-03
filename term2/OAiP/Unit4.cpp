//---------------------------------------------------------------------------


#pragma hdrstop
#include "vcl.h"
#include "Unit4.h"
#include "Unit3.h"

void StackForCalc::Push(float culcnum) {
	Calculate *tmp = new Calculate;
	tmp->num = culcnum;
	if (head == NULL) {
	   head = tmp;
	   head->next = NULL;
	   tmp = NULL;
	}
	else {
		tmp->next = head;
		head = tmp;
		tmp = NULL;
	}
}

float StackForCalc::Pop() {
	Calculate *numToReturn = head;
	head = head->next;
	float a = numToReturn->num;
	delete numToReturn;
	return a;
}

void StackForOpz::Push(char opzchr) {
	Opz *tmp = new Opz;
	tmp->chr = opzchr;
	if (head == NULL) {
	   head = tmp;
	   head->next = NULL;
	   tmp = NULL;
	}
	else {
		tmp->next = head;
		head = tmp;
		tmp = NULL;
	}
}

char StackForOpz::Pop() {
	Opz *charToReturn = head;
	head = head->next;
	char a = charToReturn->chr;
	delete charToReturn;
	return a;
}


//---------------------------------------------------------------------------

#pragma package(smart_init)
