#pragma once
#include <iostream>
using namespace std;

typedef struct avl {
	int key, size, height;
	avl* left, * right;
}AVL;

AVL* create_node(int key)
{
	AVL* node = new AVL;
	node->key = key;
	node->height = 1;
	node->size = 1;
	node->left = node->right = NULL;
	return node;
}

int height(AVL* node) //functie pt returnarea inaltimii unui nod
{
	if (node != NULL)
		return node->height;
	else
		return 0;
}

int size(AVL* node) //functie pt returnarea size-ului unui nod
{
	if (node != NULL)
		return node->size;
	else
		return 0;
}

void updateNode(AVL* node) //functie pt actualizare size si height pt un nod
{
	if (node != NULL)
	{
		node->height =1+ max(height(node->left), height(node->right));
		node->size = 1 + size(node->left) + size(node->right);
	}
}

AVL* rightRotate(AVL* nod1) //rotatie dreapta pt dezechilibru pe partea stanga
{
	AVL* nod2 = nod1->left;
	AVL* nod3 = nod2->right;

	nod2->right = nod1;
	nod1->left = nod3;

	updateNode(nod1);
	updateNode(nod2);
	return nod2;
}

AVL* leftRotate(AVL* nod2) //rotatie stanga pt dezechilibru pe partea dreapta
{
	AVL* nod1 = nod2->right;
	AVL* nod3 = nod1->left;

	nod1->left = nod2;
	nod2->right = nod3;

	updateNode(nod1);
	updateNode(nod2);
	return nod1;
}

AVL* balance(AVL* node)
{
	if (node == NULL)
		return NULL;
	int balance = height(node->left) - height(node->right);
	if (balance > 1) //dezechilibru pe stanga
	{
		if (height(node->left->left) >= height(node->left->right)) //subarborele stang al copilului stang e mai mare sau egal cu subarborele drept
			return rightRotate(node); //o singura rotatie dreapta
		else //subarborele drept al copilului stanga e mai inalt decat subarborele stang
		{
			node->left = leftRotate(node->left);   
			return rightRotate(node); //rotatie stanga-dreapta
		}
	}
	if (balance < -1) //dezechilibru pe partea dreapta
	{
		if (height(node->right->right) >= height(node->right->left)) //subarborele drept al copilului dreapta e mai inalt sau egal cu subarborele stang
		{
			return leftRotate(node); //o singura rotatie pe stanga
		}
		else //subarborele stang al copilului dreapta e mai inalt decat subarborele drept
		{
			node->right = rightRotate(node->right);
			return leftRotate(node); //rotatie dreapta-stanga
		}
	}
	return node;
}

AVL* insert(AVL* root, int key)
{
	if (root == NULL)
		return create_node(key);
	if (key < root->key)
		root->left = insert(root->left, key);
	else if (key > root->key)
	{
		root->right = insert(root->right, key);
	}
	else return root;
	updateNode(root);
	return balance(root);
}

AVL* find_min(AVL* root)
{
	while (root->left != NULL)
		root = root->left;
	return root;
}

AVL* delete_node(AVL* root, int key)
{
	if (root == NULL)
		return NULL;
	if (key < root->key)
		root->left = delete_node(root->left, key); // cautare nod in subarboreel stang sau drept in functie de cheie
	else if (key > root->key)
		root->right = delete_node(root->right, key);
	else
	{
		if (root->left == NULL || root->right == NULL)//caz cu un copil sau niciunul
		{
			AVL* temp = (root->left != NULL ? root->left : root->right);
			delete root;
			return temp;
		}
		AVL* temp = find_min(root->right); //aflam succesorul -- cazul cu 2 copii
		root->key = temp->key;
		root->right = delete_node(root->right, temp->key);
	}
	updateNode(root);
	return balance(root);
}

AVL* os_selectAVL(AVL* root,int i)
{
	if (root == NULL)
		return NULL;
	int left_size = size(root->left);
	if (i == left_size + 1)
	{
		return root;
	}
	else if (i <= left_size)
	{
		return os_selectAVL(root->left, i);
	}
	else
	{
		return os_selectAVL(root->right, i - left_size - 1);
	}
}

void prettyPrint(AVL* root, int tabs) {
	if (!root) return;

	prettyPrint(root->right, tabs + 1); 

	for (int i = 0; i < tabs; ++i) {
		cout << "    ";
	}
	cout << root->key << " (s:" << root->size << ",h:" << root->height << ")" << endl;

	prettyPrint(root->left, tabs + 1);
}

void demoAVL() {
	AVL* root = NULL;

	cout << "Construire AVL Tree" << endl;
	int arr[] = { 10, 20, 30, 40, 50, 25 };
	int n = sizeof(arr) / sizeof(arr[0]);

	// inserare elemente in avl tree
	for (int i = 0; i < n; i++) {
		cout << "Inserare: " << arr[i] << endl;
		root = insert(root, arr[i]);
		prettyPrint(root,0);
		cout << "-------------------------------------" << endl;
	}

	//prettyPrint(root, 0);
	cout << "Selectare elemente cu os-select:" << endl;
	for (int i = 1; i <= n; i++) {
		AVL* node = os_selectAVL(root, i);
		if (node) {
			cout << i << "-lea cel mai mic element: " << node->key << endl;
		}
		else {
			cout << i << "-lea cel mai mic element nu a fost gasit!" << endl;
		}
	}

	cout << "Stergere elemente din AVL Tree" << endl;
	int deleteKeys[] = { 30, 40, 50 };
	for (int i = 0; i < 3; i++) {
		cout << "Stergere: " << deleteKeys[i] << endl;
		root = delete_node(root, deleteKeys[i]);
		prettyPrint(root,0);
		cout << "-------------------------------------" << endl;
	}
}

