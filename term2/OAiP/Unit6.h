//---------------------------------------------------------------------------

#ifndef Unit6H
#define Unit6H
#include "vcl.h"


struct Data {
	int num;
	Data *next;
	Data *prev;
};

class Queue {
	protected:
	Data *head, *tail;
	public:
	Queue() {
		head = NULL;
		tail = NULL;
	}
	void Push(int addnum);
	int Pop();
	void Show(TListBox *ListBox1);
	bool IsEmpty();
	void Clear();
};

class IntNums : public Queue {
	Data *headForPos, *headForNeg, *tailForPos, *tailForNeg;
	public:
	IntNums() : Queue() {
		headForPos = NULL;
		headForNeg = NULL;
		tailForPos = NULL;
		tailForNeg = NULL;
	}
	void GenRandInts(int randm);
	void DivideOnTwo(TListBox *ListBox2, TListBox *ListBox3);
};











//---------------------------------------------------------------------------
#endif
