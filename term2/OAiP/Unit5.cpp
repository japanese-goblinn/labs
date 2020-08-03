//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit6.h"
#include "Unit6.cpp"
#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TForm5 *Form5;
IntNums qmem;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm5::CSpinEdit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
		try {
		  int z = StrToInt(CSpinEdit1->Text);
		} catch (EConvertError &) {
			ShowMessage("ÎØÈÁÊÀ ÂÂÎÄÀ");
			return;
		}
		ListBox1->Clear();
		int z = StrToInt(CSpinEdit1->Text);
		qmem.GenRandInts(z);
		qmem.Show(ListBox1);
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm5::Button1Click(TObject *Sender)
{
	ListBox2->Clear();
	ListBox3->Clear();
	qmem.DivideOnTwo(ListBox2, ListBox3);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button3Click(TObject *Sender)
{
    if (qmem.IsEmpty()) {
	  ShowMessage("î÷åðåäü ïóñòà");
	}
	else {
	   ShowMessage("î÷åðåäü ÍÅ ïóñòà");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button2Click(TObject *Sender)
{
	try {
	   qmem.Push(StrToInt(Edit1->Text));
	   ShowMessage("ÄÎÁÀÂËÅÍÈÅ ÏÐÎÈÇÎØËÎ ÓÑÏÅØÍÎ");
	} catch (EConvertError &) {
		ShowMessage("ÎØÈÁÊÀ ÂÂÎÄÀ");
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button4Click(TObject *Sender)
{
	if (!qmem.IsEmpty()) {
		qmem.Clear();
		ShowMessage("Î×ÈÑÒÊÀ ÏÐÎÈÇÎØËÀ ÓÑÏÅØÍÎ");
	}
	else {
		ShowMessage("Î×ÅÐÅÄÁ ÏÓÑÒÀ, Î×ÈÑÒÊÀ ÍÅÂÎÇÌÎÆÍÀ");
    }

}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button5Click(TObject *Sender)
{
	if (!qmem.IsEmpty()) {
	   qmem.Pop();
		ShowMessage("ÓÄÀËÅÍÈÅ ÏÐÎÈÇÎØËÎ ÓÑÏÅØÍÎ");
	}
	else {
        ShowMessage("ÓÄÀËÅÍÈÅ ÍÅ ÏÐÎÈÇÎØËÎ, Î×ÅÐÅÄÜ ÏÓÑÒÀ");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button6Click(TObject *Sender)
{
	ListBox1->Clear();
	qmem.Show(ListBox1);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
      Button2->SetFocus();
	}
}
//---------------------------------------------------------------------------

