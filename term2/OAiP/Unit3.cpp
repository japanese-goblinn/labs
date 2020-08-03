//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit3.h"
#include "Unit4.h"
#include "Unit4.cpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

StackForOpz element;
StackForCalc calc;
bool globalCheck = false;

void __fastcall TForm3::Button1Click(TObject *Sender)
{
	AnsiString opz = Edit1->Text;
	AnsiString newopz;
	newopz += "|";
	newopz += opz;
	newopz += "|";
	int cnt = newopz.Length();
	AnsiString endopz;
	element.Push(newopz[1]);
	for (int i = 2; i <= cnt ; i++)
	{
		Opz *top = element.head;
		if (newopz[i] == 'a' || newopz[i] == 'b' || newopz[i] == 'c' || newopz[i] == 'd' || newopz[i] == 'e')
		{
			endopz += newopz[i];
		}
		if (top->chr == '|')
		{
		   if (newopz[i] == '+' || newopz[i] == '-' || newopz[i] == '*' || newopz[i] == '/' ||
			newopz[i] == '(')
		   {
			  element.Push(newopz[i]);
		   }
		   else if (newopz[i] == '|')
		   {
				break;
		   }
		   else if (newopz[i] == ')')
		   {
				ShowMessage("ÎØÈÁÊÀ");
				break;
		   }
		}
		else if (top->chr == '+')
		{
		  if (newopz[i] == '|' || newopz[i] == '+' || newopz[i] == '-')
		  {
			 char a = element.Pop();
			 endopz += a;
			 element.Push(newopz[i]);
		  }
		  else if (newopz[i] == '*' || newopz[i] == '/' || newopz[i] == '(')
		  {
			 element.Push(newopz[i]);
		  }
		  else if (newopz[i] == ')')
		  {
			  Opz *tp = element.head;
			  while (tp)
			  {
				 if (tp->chr == '(') {
					element.Pop();
					break;
				 }
				 char a = element.Pop();
				 endopz += a;
				 tp = tp->next;
			  }
		  }
		}
		else if (top->chr == '-')
		{
		   if (newopz[i] == '|' || newopz[i] == '+' || newopz[i] == '-')
		   {
			 char a = element.Pop();
			 endopz += a;
			 element.Push(newopz[i]);
		   }
		   else if (newopz[i] == '*' || newopz[i] == '/' || newopz[i] == '(')
		   {
			 element.Push(newopz[i]);
		   }
		   else if (newopz[i] == ')') {
			 Opz *tp = element.head;
			 while (tp)
			 {
				if (tp->chr == '(')
				{
					element.Pop();
					break;
				}
				char a = element.Pop();
				endopz += a;
				tp = tp->next;
			 }
		  }
		}
		else if (top->chr == '*')
		{
		   if (newopz[i] == '|' || newopz[i] == '+' || newopz[i] == '-' || newopz[i] == '*' ||
		   newopz[i] == '/')
		   {
			 char a = element.Pop();
			 endopz += a;
			 element.Push(newopz[i]);
		   }
		   else if (newopz[i] == '(')
		   {
			 element.Push(newopz[i]);
		   }
		   else if (newopz[i] == ')') {
				Opz *tp = element.head;
				while (tp)
				{
				   if (tp->chr == '(') {
					 element.Pop();
					 break;
				   }
				   char a = element.Pop();
				   endopz += a;
				   tp = tp->next;
				}
		  }
		}
		else if (top->chr == '/')
		{
		   if (newopz[i] == '|' || newopz[i] == '+' || newopz[i] == '-' || newopz[i] == '*' ||
		   newopz[i] == '/')
		   {
			 char a = element.Pop();
			 endopz += a;
			 element.Push(newopz[i]);
		   }
		   else if (newopz[i] == '(')
		   {
			 element.Push(newopz[i]);
		   }
		   else if (newopz[i] == ')') {
				Opz *tp = element.head;
				while (tp)
				{
				   if (tp->chr == '(') {
					 element.Pop();
					 break;
				   }
				   char a = element.Pop();
				   endopz += a;
				   tp = tp->next;
				}
		  }
		}
		else if (top->chr == '(')
		{
		   if (newopz[i] == '|')
		   {
			  break;
		   }
		   if (newopz[i] == '+' || newopz[i] == '-' || newopz[i] == '*' || newopz[i] == '/' ||
		   newopz[i] == '(')
		   {
			  element.Push(newopz[i]);
		   }
		   else if (newopz[i] == ')')
		   {
			  element.Pop();
			  continue;
		   }
		}
		else
		{
			ShowMessage("ÍÅÏÐÀÂÈËÜÍÛÉ ÂÂÎÄ");
			return;
        }
	}
	Opz *top = element.head;
	while (top) {
		if (top->chr != '|') {
		   char a = element.Pop();
		   endopz += a;
		   break;
		}
		element.Pop();
		top = top->next;
	}
	Edit2->Text = endopz;
	globalCheck = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button2Click(TObject *Sender)
{
	if (!globalCheck) {
	   ShowMessage("ÂÛÐÀÆÅÍÈÅ ÍÅ ÏÅÐÅÂÅÄÅÍÎ Â ÎÏÇ ÂÛ×ÈÑËÅÍÈÅ ÍÅ ÂÎÇÌÎÆÍÎ");
	   return;
	}
	try {
		float a = StrToFloat(Edit3->Text);
		float b = StrToFloat(Edit4->Text);
		float c = StrToFloat(Edit5->Text);
		float d = StrToFloat(Edit6->Text);
		float e = StrToFloat(Edit7->Text);
	}
	catch (EConvertError &) {
		ShowMessage("ÎØÈÁÊÀ ÂÂÎÄÀ");
		return;
	}
	float a = StrToFloat(Edit3->Text);
	float b = StrToFloat(Edit4->Text);
	float c = StrToFloat(Edit5->Text);
	float d = StrToFloat(Edit6->Text);
	float e = StrToFloat(Edit7->Text);
	AnsiString expression = Edit2->Text;
	int cnt = expression.Length();
	for (int i = 1; i <= cnt; i++) {
		if (expression[i] == 'a') {
			calc.Push(a);
		}
		else if (expression[i] == 'b') {
			calc.Push(b);
		}
		else if (expression[i] == 'c') {
			calc.Push(c);
		}
		else if (expression[i] == 'd') {
			calc.Push(d);
		}
		else if (expression[i] == 'e') {
			calc.Push(e);
		}
		else if (expression[i] == '+') {
			float second = calc.Pop();
			float first = calc.Pop();
			float res = first + second;
			calc.Push(res);
		}
		else if (expression[i] == '-') {
			float second = calc.Pop();
			float first = calc.Pop();
			float res = first - second;
			calc.Push(res);
		}
		else if (expression[i] == '*') {
			float second = calc.Pop();
			float first = calc.Pop();
			float res = first * second;
			calc.Push(res);
		}
		else if (expression[i] == '/') {
			float second = calc.Pop();
			float first = calc.Pop();
			float res = first / second;
			calc.Push(res);
		}
	}
	float endres = calc.Pop();
	ShowMessage("ÐÅÇÓËÜÒÀÒ ÂÛ×ÈÑËÅÍÈß: " + FloatToStr(endres));
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
	   Button1->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Edit3KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if (Key == 13) {
	   Edit4->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Edit4KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
	   Edit5->SetFocus();
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm3::Edit5KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
	   Edit6->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Edit6KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
	   Edit7->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Edit7KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
	   Button2->SetFocus();
	}
}
//---------------------------------------------------------------------------

