//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
#include "vcl.h"

struct Opz {
	char chr;
	Opz *next;
};

class StackForOpz {
	public:
	Opz *head;
	StackForOpz() {
		head = NULL;
	}
	void Push(char opzchr);
	char Pop();
};

struct Calculate {
	float num;
	Calculate *next;
};

class StackForCalc {
	 public:
	 Calculate *head;
	 StackForCalc() {
		 head = NULL;
	 }
	 void Push(float calcnum);
	 float Pop();
};






//---------------------------------------------------------------------------
#endif
