//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit7.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm7 *Form7;


String names[] = {"Jhon", "Luna", "Harry", "Jacob", "Oliver", "Tomas", "Olivia", "Sophie", "Leo", "Ann", "Mia", "Felix", "Elon", "Grace", "Lily"};
int nums[] = {1,4,8,14,2,12,9,13,3,5,10,7,11,6,15};

//Îïğåäåëèòü ÷èñëî ëèñòüåâ íà êàæäîì óğîâíå äåğåâà

struct Node {
	int key;
	String name;
	int heigh;
	Node *left, *right;
	Node(int k, String n) {
	   key = k;
	   name = n;
	   left = right = NULL;
	   heigh = 1;
	}
};

class Tree {
	public:
	Node *Root;
	Tree() {
		Root = NULL;
	}
	int Height(Node *node);
	int Bfactor(Node *node);
	void CheckHeight(Node *node);
	Node* RotateLeft(Node *root);
	Node* RotateRight(Node *root);
	Node* Balance(Node *node);
	Node* Add(Node *root, int key, String name);
	Node* Delete(Node *root, int key);
	Node* FindMin(Node *node);
	Node* RemoveMin(Node *node);
	void Show(TTreeView *TreeView1, TTreeNode *node, Node *root);
	void InfixShow(Node *node, TMemo *Memo1);
	void PrefixShow(Node *node, TMemo *Memo1);
	void PostfixShow(Node *node, TMemo *Memo1);
	Node* Search(Node *root, int key);
};

class LeavesCount : public Tree {
	public:
	void Lcounter(Node *root);
};

LeavesCount obj;

int Tree::Height(Node *node) {
	if (node) {
	   return node->heigh;
	}
	else {
		return 0;
	}
}

int Tree::Bfactor(Node *node) {
	return Height(node->right) - Height(node->left);
}

void Tree::CheckHeight(Node *node) {
	int hl = Height(node->left);
	int hr = Height(node->right);
	if (hr < hl) {
		node->heigh = hl + 1;
	}
	else {
		node->heigh = hr + 1;
    }
}

Node *Tree::RotateRight(Node *root) {
	Node *newroot = root->left;
	root->left = newroot->right;
	newroot->right = root;
	CheckHeight(root);
	CheckHeight(newroot);
	return newroot;
}

Node *Tree::RotateLeft(Node *root) {
	Node *newroot = root->right;
	root->right = newroot->left;
	newroot->left = root;
	CheckHeight(root);
	CheckHeight(newroot);
	return newroot;
}

Node *Tree::Balance(Node *node) {
   CheckHeight(node);
   if (Bfactor(node) == 2)
   {
		if (Bfactor(node->right) < 0) {
			node->right = RotateRight(node->right);
		}
		return RotateLeft(node);

   }
   if (Bfactor(node) == -2)
   {
	   if (Bfactor(node->left) > 0) {
			node->left = RotateLeft(node->left);
	   }
	   return RotateRight(node);
   }
   return node;
}

Node *Tree::Add(Node *root, int key, String name) {
	if (!root)
	{
		return new Node(key, name);
	}
	if (key < root->key)
	{
	   root->left = Add(root->left, key, name);
	}
	else if (key > root->key)
	{
	   root->right = Add(root->right, key, name);
	}
	return Balance(root);
}

Node *Tree::FindMin(Node *node) {
   if (node->left) {
	   return FindMin(node->left);
   }
   else {
	   return node;
   }
}

Node *Tree::RemoveMin(Node *node) {
	if (node->left == NULL) {
		return node->right;
	}
	node->left = RemoveMin(node->left);
	return Balance(node);
}

Node *Tree::Delete(Node *root, int key) {
	if (!root) {
	   return NULL;
	}
	if (key < root->key) {
	   root->left = Delete(root->left, key);
	}
	else if (key > root->key) {
		root->right = Delete(root->right, key);
	}
	else {
		 Node *l = root->left;
		 Node *r = root->right;
		 delete root;
		 if (!r) return l;
		 Node *min = FindMin(r);
		 min->right = RemoveMin(r);
		 min->left = l;
		 return Balance(min);
	}
	return Balance(root);
}

Node *Tree::Search(Node *root, int key) {
	if (!root) {
		return NULL;
	}
	else if (key < root->key) {
		Search(root->left, key);
	}
	else if (key > root->key) {
		Search(root->right, key);
	}
	else {
		return root;
	}
}

void Tree::Show(TTreeView *TreeView1, TTreeNode *node, Node *root) {
	if (!root) {
		return;
	}
	node = TreeView1->Items->AddChild(node, IntToStr(root->key) +  " (" + root->name + ")");
	if (root->left) {
		Show(TreeView1, node, root->left);
	}
	if (root->right) {
		Show(TreeView1, node, root->right);
	}
}

void Tree::InfixShow(Node *node, TMemo *Memo1) {
	if (!node) {
		return;
	}
	InfixShow(node->left, Memo1);
	Memo1->Lines->Add(IntToStr(node->key) + " " + node->name);
	InfixShow(node->right, Memo1);
}

void Tree::PrefixShow(Node *node, TMemo *Memo1) {
   if (!node) {
	   return;
   }
   Memo1->Lines->Add(IntToStr(node->key) + " " + node->name);
   PrefixShow(node->left, Memo1);
   PrefixShow(node->right, Memo1);
}

void Tree::PostfixShow(Node *node, TMemo *Memo1) {
	if (!node) {
	  return;
	}
	PostfixShow(node->left, Memo1);
	PostfixShow(node->right, Memo1);
	Memo1->Lines->Add(IntToStr(node->key) + " " + node->name);
}

int amountOfLeaves[30] = {0};
int globalH = 0;
int count = 0;

void LeavesCount::Lcounter(Node *node) {
	count++;
	if (!node) {
		return;
	}
	if (node->left == NULL && node->right == NULL) {
		amountOfLeaves[count - 1] += 1;
		return;
	}
	Lcounter(node->left);
	count--;
	Lcounter(node->right);
	count--;
}


//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm7::Button1Click(TObject *Sender)
{
	StringGrid1->Cells[0][0] = "Íîìåğ";
	StringGrid1->Cells[1][0] = "Èìÿ";
	for (int i = 1; i < StringGrid1->RowCount; i++) {
		 StringGrid1->Cells[0][i] = nums[i-1];
		 StringGrid1->Cells[1][i] = names[i-1];
		 obj.Root = obj.Add(obj.Root, StrToInt(StringGrid1->Cells[0][i]), StringGrid1->Cells[1][i]);
	}
	Button1->Enabled = false;
	Button2->Enabled = true;
	Button3->Enabled = true;
	Button4->Enabled = true;
	Button5->Enabled = true;
	Button6->Enabled = true;
	Button7->Enabled = true;
	Button8->Enabled = true;
	Button9->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button2Click(TObject *Sender)
{
	String name = Edit1->Text;
	try {
	   int key = StrToInt(Edit2->Text);
	} catch (EConvertError &) {
		ShowMessage("ÎØÈÁÊÀ");
		return;
	}
	int key = StrToInt(Edit2->Text);
	Node *srch = obj.Search(obj.Root, key);
	if (!srch) {
		obj.Root = obj.Add(obj.Root, key, name);
		Memo1->Lines->Add(">>İËÅÌÅÍÒ ÄÎÁÀÂËÅÍ");
		TreeView1->Items->Clear();
	}
	else if (srch->key == key) {
	  Memo1->Lines->Add(">>İËÅÌÅÍÒ Ñ ÒÀÊÈÌ ÊËŞ×ÅÌ ÓÆÅ ÑÓÙÅÑÒÂÓÅÒ");
	  return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button3Click(TObject *Sender)
{
	TreeView1->Items->Clear();
	TTreeNode *node = TreeView1->Items->GetFirstNode();
	obj.Show(TreeView1, node, obj.Root);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
        Edit2->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Edit2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
        Button2->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Edit2Change(TObject *Sender)
{
	if (Edit2->Text == "0") {
		ShowMessage("ÎØÈÁÊÀ ÂÂÎÄÀ");
		Edit2->Clear();
		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button4Click(TObject *Sender)
{
	try {
	   int key = StrToInt(Edit3->Text);
	} catch (EConvertError &) {
		ShowMessage("ÎØÈÁÊÀ");
		return;
	}
	int key = StrToInt(Edit3->Text);
	Node *srch = obj.Search(obj.Root, key);
	if (!srch) {
		 Memo1->Lines->Add(">>İËÅÌÅÍÒÀ Ñ ÒÀÊÈÌ ÊËŞ×ÅÌ ÍÅ ÑÓÙÅÑÒÂÓÅÒ");
		 return;
	}
	else if (srch->key == key) {
		obj.Root = obj.Delete(obj.Root, key);
		Memo1->Lines->Add(">>İËÅÌÅÍÒ ÓÄÀËÅÍ");
		TreeView1->Items->Clear();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Edit3Change(TObject *Sender)
{
	if (Edit3->Text == "0") {
		ShowMessage("ÎØÈÁÊÀ ÂÂÎÄÀ");
		Edit3->Clear();
		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Edit3KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == 13) {
        Button4->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button5Click(TObject *Sender)
{
	try {
	   int key = StrToInt(Edit4->Text);
	} catch (EConvertError &) {
		ShowMessage("ÎØÈÁÊÀ");
		return;
	}
	int key = StrToInt(Edit4->Text);
	Node *srch = obj.Search(obj.Root, key);
	if (!srch) {
		Memo1->Lines->Add(">>İËÅÌÅÍÒ Ñ ÒÀÊÈÌ ÊËŞ×ÅÌ ÍÅ ÍÀÉÄÅÍ");
		return;
	}
	else if (srch->key == key) {
		Memo1->Lines->Add("Êëş÷ " + IntToStr(srch->key) + ", Èìÿ " + srch->name);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Edit4Change(TObject *Sender)
{
	if (Edit4->Text == "0") {
		ShowMessage("ÎØÈÁÊÀ ÂÂÎÄÀ");
		Edit3->Clear();
		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button6Click(TObject *Sender)
{
	Memo1->Clear();
	obj.InfixShow(obj.Root, Memo1);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button7Click(TObject *Sender)
{
	Memo1->Clear();
	obj.PrefixShow(obj.Root, Memo1);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button8Click(TObject *Sender)
{
	Memo1->Clear();
    obj.PostfixShow(obj.Root, Memo1);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::Button9Click(TObject *Sender)
{
	globalH = obj.Root->heigh;
	obj.Lcounter(obj.Root);
	for (int i = 0, j = 0; i < globalH; i++, j++) {
		Memo1->Lines->Add("Êîëè÷åñòâî ëèñòüåâ íà " + IntToStr(i+1) + " óğîâíå = " + IntToStr(amountOfLeaves[j]));
	}
	for (int i = 0; i < globalH; i++) {
		amountOfLeaves[i] = 0;
	}
	count = 0;
}
//---------------------------------------------------------------------------

