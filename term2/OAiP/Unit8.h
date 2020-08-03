//---------------------------------------------------------------------------

#ifndef Unit8H
#define Unit8H
#include "vcl.h"

//ќпределить число листьев на каждом уровне дерева

struct Node {
	int key;
	int heigh;
	Node *left, *right;
	Node(int k) {
	   key = k;
	   left = right = NULL;
	   heigh = 1;

	}
};

class Tree {
	Node *root;
	Tree() {
		root = NULL;
	}
	void Create();
	void Add();
	int Find();
};




//---------------------------------------------------------------------------
#endif
