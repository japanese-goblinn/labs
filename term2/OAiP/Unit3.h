//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *Edit1;
	TLabel *Label2;
	TEdit *Edit2;
	TButton *Button1;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	TEdit *Edit6;
	TEdit *Edit7;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TButton *Button2;
	TLabel *Label9;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit3KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit4KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit5KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit6KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit7KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
