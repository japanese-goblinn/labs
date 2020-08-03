//---------------------------------------------------------------------------

#ifndef Unit7H
#define Unit7H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TForm7 : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *StringGrid1;
	TMemo *Memo1;
	TButton *Button1;
	TEdit *Edit1;
	TEdit *Edit2;
	TButton *Button2;
	TTreeView *TreeView1;
	TLabel *Label1;
	TLabel *Label2;
	TButton *Button3;
	TEdit *Edit3;
	TButton *Button4;
	TEdit *Edit4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit2Change(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Edit3Change(TObject *Sender);
	void __fastcall Edit3KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Edit4Change(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm7(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm7 *Form7;
//---------------------------------------------------------------------------
#endif
