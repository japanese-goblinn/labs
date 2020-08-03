//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TLabel *Label1;
	TEdit *Edit1;
	TLabel *Label2;
	TEdit *Edit2;
	TEdit *Edit3;
	TLabel *Label3;
	TLabel *Label4;
	TButton *Button1;
	TButton *Button2;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *Edit4;
	TButton *Button3;
	TLabel *Label7;
	TEdit *Edit5;
	TButton *Button4;
	TLabel *Label8;
	TEdit *Edit6;
	TButton *Button5;
	TButton *Button6;
	TOpenDialog *OpenDialog1;
	TLabel *Label9;
	TEdit *Edit7;
	TLabel *Label10;
	TButton *Button7;
	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit3KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
