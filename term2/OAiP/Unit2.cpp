//---------------------------------------------------------------------------

#include "Unit2.h"
#include <vcl.h>
#pragma hdrstop
int idd = 0;
int cnt = 0;

void List::AddProd(TEdit *Edit1, TEdit *Edit2, TEdit *Edit3, TMemo *Memo1) {
	if (Edit1->Text == "" || Edit2->Text == "" || Edit3->Text == "") {
	   Memo1->Lines->Add(">>¬¬≈ƒ≈Õ€ Õ≈¬≈–Õ€≈ ƒ¿ÕÕ€≈...");
	   return;
	}
	if (!cnt) {
		Memo1->Lines->Add(">>ƒŒ¡¿¬À≈Õ»≈ Õ≈¬Œ«ÃŒ∆ÕŒ ¡≈« »Õ»÷»¿À»«¿÷» ƒ¿ÕÕ€’...");
		return;
	}
	Data *tmp = new Data;
	tmp->name = Edit1->Text;
	tmp->amount = StrToInt(Edit2->Text);
	tmp->factoryNum = StrToInt(Edit3->Text);
	tmp->ID = idd;
	idd++;
	tail->next = tmp;
	tail = tail->next;
	tail->next = NULL;
	Memo1->Lines->Add(">>œ–Œƒ” “ ƒŒ¡¿¬À≈Õ ¬ —œ»—Œ ...");
}

void List::ShowList(TMemo *Memo1) {
	if (!cnt) {
	   Memo1->Lines->Add("—œ»—Œ  œ”—“");
	}
	Data *product = head;
	while (product) {
		Memo1->Lines->Add(product->name + " " + IntToStr(product->amount) + " " + IntToStr(product->factoryNum));
		product = product->next;
	}
}

void List::ShowProductInfo(TMemo *Memo1, TEdit *Edit4, bool check) {
	check = false;
	Data *product = head;
	Memo1->Lines->Add(">>»Õ‘Œ–Ã¿÷»ﬂ Œ œ–Œƒ” ÷»» '" + Edit4->Text + "':");
	while (product) {
		if (product->name == Edit4->Text) {
		   check = true;
		   Memo1->Lines->Add(" ÓÎÎË˜ÂÒÚ‚Ó: " + IntToStr(product->amount) + "," + " ÕÓÏÂ ÷Âı‡: " + IntToStr(product->factoryNum));
		}
		product = product->next;
	}
	if (!check) {
	   Memo1->Lines->Add("“¿ ¿ﬂ œ–Œƒ” ÷»ﬂ Õ≈ ¬€œ”— ¿≈“—ﬂ");
	}
}

void List::ShowFactoryInfo(TMemo *Memo1, TEdit *Edit5) {
	Data *product = head;
	int checker = 0;
	while (product) {
		if (product->factoryNum == Edit5->Text) {
			checker++;
			Data *search = head;
			Memo1->Lines->Add(">>»Õ‘Œ–Ã¿÷»ﬂ Œ œ–Œƒ” ÷»» ÷≈’¿ π" + IntToStr(product->factoryNum) + ":");
			while (search) {
			   if (search->factoryNum == product->factoryNum) {
				   Memo1->Lines->Add("Õ‡Á‚‡ÌËÂ: '" + search->name + "', " + " ÓÎÎË˜ÂÒÚ‚Ó: " + IntToStr(search->amount));
			   }
			   search = search->next;
			}
			break;
		}
		product = product->next;
	}
	if (!checker) {
		Memo1->Lines->Add(">>÷≈’¿ — “¿ »Ã ÕŒÃ≈–ŒÃ Õ≈ —”Ÿ≈—“¬”≈“...");
	}
}

bool CheckIfNumInArray(int *mass, int num) {
   for (int i = 0; i < 50; i++) {
	  if (mass[i] == num) {
		  return false;
	  }
	  else {
		  continue;
	  }
   }
   return true;
}

void List::CalcProductAmount(TMemo *Memo1, TEdit *Edit6) {
	int add = 0;
	int cur = 0;
	int totalAmount[50] = {0};
	int mass[50] =  {0};
	int counter = 0;
	Data *product = head;
	while (product)
	{
		 if (CheckIfNumInArray(mass, product->factoryNum)) {
			mass[cur] = product->factoryNum;
			cur++;
		 }
		 product = product->next;
	}
	int checker = 0;
	product = head;
	while (product)
	{
		for (int i = 0; i < cur; i++)
		{
		   if (product->factoryNum == mass[i])
		   {
			   Data *search = head;
			   while (search)
			   {
					if ((search->factoryNum == product->factoryNum) && (search->ID == product->ID))
					{
					   totalAmount[i] += search->amount;
					}
					search = search->next;
			   }
			   break;
		   }

		}
		product = product->next;
	 }
	 product = head;
	 for (int i = 0; i < cur; i++)
	 {
		if (totalAmount[i] >= StrToInt(Edit6->Text))
		{
			checker++;
			while (product)
			{
			   if (product->factoryNum == mass[i]) {
				   Memo1->Lines->Add(">>»Õ‘Œ–Ã¿÷»ﬂ Œ ÷≈’≈ π" + IntToStr(product->factoryNum) + "  Œ“Œ–€… ¬€œ”— ¿≈“ " + IntToStr(totalAmount[i]) + " œ–Œƒ” ÷»» ¬ŒŒ¡Ÿ≈Ã:");
				   Data *search = head;
				   while (search)
				   {
					   if (search->factoryNum == product->factoryNum) {
						   Memo1->Lines->Add("Õ‡Á‚‡ÌËÂ œÓ‰ÛÍˆËË: '" + search->name + "',  ÓÎÎË˜ÂÒÚ‚Ó: " + IntToStr(search->amount));
					   }
					   search = search->next;
				   }
				   break;
			   }
			   product = product->next;
			}
		}
	 }
	 if (!checker) {
		  Memo1->Lines->Add(">>“¿ Œ√Œ ÷≈’¿ Õ≈ —”Ÿ≈—“¬”≈“...");
	 }
}

void List::AddNew(TOpenDialog *OpenDialog1, TMemo *Memo1) {
	TStringList *ProdList = new TStringList;
	if (cnt) {
		Memo1->Lines->Add(">>ƒ¿ÕÕ€≈ ”∆≈ ¡€À» »Õ»÷»¿À»«»–Œ¬¿ÕÕ€, œŒ¬“Œ–ŒÕŒ≈ ƒ≈…—“¬»≈ Õ≈¬Œ«ÃŒ∆ÕŒ...");
		return;
	}
	try {
		if (OpenDialog1->Execute()) {
		   Memo1->Lines->Add(">>»ÃœŒ–“ ƒ¿ÕÕ€’ œ–Œÿ≈À ”—œ≈ÿÕŒ...");
		}
		else {
			throw false;
        }
		cnt++;
		ProdList->LoadFromFile(OpenDialog1->FileName);
		for (int i = 0; i < 15; i++) {
			Data *tmp = new Data;
			tmp->name = ProdList->Strings[i];
			i++;
			tmp->amount = StrToInt(ProdList->Strings[i]);
			i++;
			tmp->factoryNum = StrToInt(ProdList->Strings[i]);
			tmp->ID = idd;
			idd++;
			if (head == NULL) {
				head = tmp;
				tail = tmp;
				tmp = NULL;
			}
			else {
				tail->next = tmp;
				tail = tail->next;
				tail->next = NULL;
			}
		}
	}
	catch (bool a) {
		Memo1->Lines->Add(">>ƒ¿ÕÕ€≈ Õ≈ ¡€À» »Õ»÷»¿À»«»–Œ¬¿ÕÕ€...");
	}
}

void List::DeleteProd(TMemo *Memo1, TEdit *Edit7) {
	int counter = 0;
	Data *current = head;
	while (current)
	{
		if (current->name == Edit7->Text && head->name != Edit7->Text) {
			counter++;
			Data *cur = head;
			Data *previous;
			while (cur->name != Edit7->Text)
			{
				 previous = cur;
				 cur = cur->next;
			}
			Data *tmp;
			previous->next = cur->next;
			tmp = cur;
			cur = cur->next;
			delete tmp;
			Memo1->Lines->Add(">>ƒ¿ÕÕ€≈ Œ œ–Œƒ” ÷»» ”ƒ¿À≈Õ€...");
		}
		if (current->name == Edit7->Text && current->next == NULL) {
			counter++;
			Data *element;
			Data *prev;
			element = head;
			while(element->next != NULL)
			{
				prev = element;
				element = element->next;
			}
			tail = prev;
			tail->next = NULL;
			delete element;
			Memo1->Lines->Add(">>ƒ¿ÕÕ€≈ Œ œ–Œƒ” ÷»» ”ƒ¿À≈Õ€...");
		}
		if (current->name == Edit7->Text && head->name == Edit7->Text) {
			counter++;
			Data *temp = new Data;
			temp = head;
			head = head->next;
			delete temp;
			Memo1->Lines->Add(">>ƒ¿ÕÕ€≈ Œ œ–Œƒ” ÷»» ”ƒ¿À≈Õ€...");
		}
		current = current->next;
	}
	if (!counter) {
	   Memo1->Lines->Add(">>“¿ ¿ﬂ œ–Œƒ” ÷»ﬂ Õ≈ ¬€œ”— ¿≈“—ﬂ...");
	}
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
