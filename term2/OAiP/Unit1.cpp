//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit2.h"
#include "Unit1.h"
#include "Unit2.cpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

List product;
int check = 0;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
	   Edit2->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
       Edit3->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit3KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
      Button1->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if (Edit3->Text <= 0) {
		Memo1->Lines->Add(">>мебнглнфмши мнлеп жеую...");
		return;
	}
	try {
	  StrToInt(Edit2->Text);
	} catch (EConvertError &) {
		ShowMessage("ньхайю ббндю");
		return;
	}
	try {
	  StrToInt(Edit3->Text);
	} catch (EConvertError &) {
		ShowMessage("ньхайю ббндю");
		return;
	}
	product.AddProd(Edit1, Edit2, Edit3, Memo1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	Memo1->Clear();
	Memo1->Lines->Add(">>яохянй опндсйжхх...");
	product.ShowList(Memo1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1KeyPress(TObject *Sender, wchar_t &Key)
{
      Edit1->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	product.ShowProductInfo(Memo1, Edit4, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	try {
	  StrToInt(Edit5->Text);
	} catch (EConvertError &) {
		ShowMessage("ньхайю ббндю");
		return;
	}
	product.ShowFactoryInfo(Memo1, Edit5);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button5Click(TObject *Sender)
{
	try {
	  StrToInt(Edit6->Text);
	} catch (EConvertError &) {
		ShowMessage("ньхайю ббндю");
		return;
	}
	product.CalcProductAmount(Memo1, Edit6);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button6Click(TObject *Sender)
{
	product.AddNew(OpenDialog1, Memo1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
	product.DeleteProd(Memo1, Edit7);
}
//---------------------------------------------------------------------------

