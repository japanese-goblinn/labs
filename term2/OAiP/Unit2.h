//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
#include "vcl.h"
#include "Unit1.h"

/*
��������� ���� ������ ��������� ��������� ���������� ������������.
�������� � ���������� ��������� ��������: ������������, ����������,
����� ����.
��������� ���������, �������:
������������ �������������� ���� ������ � �������������� ������� �
������������ ��������� ������;
���������� ����� ����� ������;
������ ������������ ��������� � ������� ��� ������ � ���;
������� ���������� � ���������, ����������� ����� � ������������ �������;
������� ���������� � ����, ����������� ��������� � ���������� �� ����
����������;
� ������ ������ ��������� � ������������ ��������� ��� ���������� � ���;
� ������ ������� ������ ������������ ��������� ����������� ���������� � ���.
*/

struct Data {
	String name;
	int amount;
	int factoryNum;
	int ID;
	Data *next;
};

class List {
	Data *head, *tail;
	public:
	List() {
		head = NULL;
		tail = NULL;
	}
	void AddNew(TOpenDialog *OpenDialog1, TMemo *Memo1);
	void ShowList(TMemo *Memo1);
	void ShowProductInfo(TMemo *Memo1, TEdit *Edit4, bool check);
	void ShowFactoryInfo(TMemo *Memo1, TEdit *Edit5);
	void CalcProductAmount(TMemo *Memo1, TEdit *Edit6);
	void DeleteProd(TMemo *Memo1, TEdit *Edit7);
	void AddProd(TEdit *Edit1, TEdit *Edit2, TEdit *Edit3, TMemo *Memo1);
};



//---------------------------------------------------------------------------
#endif
