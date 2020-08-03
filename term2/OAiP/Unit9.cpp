//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "vcl.h"
#include <ctime>
#include "Unit9.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm9 *Form9;

/*
 Создать хэш-таблицу из случайных целых чисел и найти в ней номер стека,
 содержащего максимальное значение ключа
*/

struct Cell {
	 int key;
	 int info;
	 Cell *next;
 };

int const m = 10;
Cell *hash[m];

class HashTable {
	public:
	int Hash(int key);
	void Push(Cell *&head, int num, int key);
	int Pop(Cell *&head);
};

class RandInts: public HashTable {
	public:
	int FindMax(Cell *head);
};


RandInts func;

void View(TMemo *Memo1) {
	String output;
	for (int i = 0; i < m; i++) {
		output += "[" + IntToStr(i) + "] ";
		Cell *frst = hash[i];
		while (frst) {
			output += frst->info;
			output += " ";
			frst = frst->next;
		}
		Memo1->Lines->Add(output);
		output = "";
   }
}

int HashTable::Hash(int key) {
	return key % 10;
}

void HashTable::Push(Cell *&head, int num, int key) {
	Cell *tmp = new Cell;
	tmp->info = num;
	tmp->key = key;
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

int HashTable::Pop(Cell *&head) {
	Cell *numToReturn = head;
	head = head->next;
	int num = numToReturn->info;
	delete numToReturn;
	return num;
}


//---------------------------------------------------------------------------
__fastcall TForm9::TForm9(TComponent* Owner)
	: TForm(Owner)
{
   Button2->Enabled = false;
   Button3->Enabled = false;
   Button4->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm9::Button1Click(TObject *Sender)
{
   srand(time(NULL));
   for (int j = 0; j < 20; j++) {
		int num =  rand() % 1000 + (-500);
		int key = rand() % 10000 + 1;
		int i = func.Hash(key);
		func.Push(hash[i], num, key);
   }
   View(Memo1);
   Button1->Enabled = false;
   Button2->Enabled = true;
   Button3->Enabled = true;
   Button4->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm9::Button2Click(TObject *Sender)
{
	try {
	   int num = StrToInt(Edit1->Text);
	   int key = StrToInt(Edit3->Text);
	}
	catch (EConvertError &) {
		ShowMessage("ОШИБКА ВВОДА");
		return;
	}
	int num = StrToInt(Edit1->Text);
	int key = StrToInt(Edit3->Text);
	int i = func.Hash(key);
	func.Push(hash[i], num, key);
	Memo1->Clear();
	View(Memo1);

}
//---------------------------------------------------------------------------

void __fastcall TForm9::Button3Click(TObject *Sender)
{
	try {
	   int num = StrToInt(Edit2->Text);
	}
	catch (EConvertError &) {
		ShowMessage("ОШИБКА ВВОДА");
		return;
	}
	int num = StrToInt(Edit2->Text);
	int keyToFind = 0;
	int cnt = 0;
	for (int i = 0; i < m; i++) {
		Cell *p = hash[i];
		while (p) {
			if (p->info == num) {
					cnt++;
					keyToFind = p->key;
					break;
				}
				p = p->next;
		}
	}
	if (!cnt) {
		ShowMessage("ЭЛЕМЕНТ НЕ НАЙДЕН!");
		return;
	}
	int i = func.Hash(keyToFind);
	Cell *del = hash[i];
	while (del->key != keyToFind && del) {
		func.Pop(hash[i]);
		del = del->next;
	}
	func.Pop(hash[i]);
	Memo1->Clear();
	View(Memo1);
}
//---------------------------------------------------------------------------

void __fastcall TForm9::Button4Click(TObject *Sender)
{
	int massOfmax[m] = {0};
	for (int i = 0; i < m; i++) {
		if (hash[i]) {
          Cell *fst = hash[i];
		  int min = hash[i]->key;
		  int max = hash[i]->key;
		  while (fst) {
			if (min >= max) {
				 max = min;
			}
			fst = fst->next;
			if (fst) {
				min = fst->key;
			}
			}
			massOfmax[i] = max;
		}
	}
	int max = massOfmax[0];
	int min = massOfmax[0];
	int maxInd = 0;
	int minInd = 0;
	for (int i = 0; i < m; i++) {
		min = massOfmax[i];
		minInd = i;
		if (min >= max) {
			max = min;
			maxInd = minInd;
		}
	}
	Memo1->Lines->Add(">>НОМЕР СТЕКА С МАКСИМАЛЬНЫМ ЗНАЧЕНИЕМ КЛЮЧА: " + IntToStr(maxInd));
}
//---------------------------------------------------------------------------

