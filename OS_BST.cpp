/* 
* @author Breban Vasile , group 8
* 
* Cerinte pentru aceasta tema:
*	1.BUILD_TREE: implementare corectă și eficientă 
*	2.OS_SELECT: implementare corectă și eficientă 
*	3.OS_DELETE: implementare corectă și eficientă
*	4.Evaluarea operațiilor de management - BUILD, SELECT, DELETE
*	5.Bonus: Implementarea utilizând AVL
* 
* Pasi de implementare:
*	-crearea bst-ului se face cu ajutorul unui sir de numere ordonat crescator,folosind o metoda divide and conquer unde luam mediana sirului(ca radacina)
*     si introducem elementele mai mici in stanga si mai mari in dreapta 
*	-cautarea unui element dupa pozitie - os_select - calculam rangul nodului ca fiind size-ul nodului stang daca exista,altfel este 0,
*	  iar daca r=i atunci am gasit elementul altfel cautam in subarborul stang sau drept
*	-stergerea unui element dupa pozitie - folosim os_select - cautam nodul in subarborele stang sau drept dupa cheie,dupa care tratam cele 3 cazuri care apar
*	 (cand nodul are copil stang sau drept,cand nu are copii si cand are 2 copii)
*	-functie de demonstrare algoritmi si de analizare a complexitatii
*
*	Complexitate : - cei 3 agloritmi au aceeiasi complexitate O(logn) = O(h) in cazul mediu si favorabil si O(n) pt caz defavorabil
*	- pe grafic apar ca fiind liniar datorita numararii operatiilor(comparari + asignari)
*/

#include <iostream>
#include "Profiler.h"
#include "AVL.h";
using namespace std;
Profiler profiler;
typedef struct bst
{
	int key,size; //size fiind numarul total de noduri al subarborelui unui nod(inclusiv root-ul)
	bst* left, * right;
}BST;


//BST* root;

BST* createNode(int value,int size,Operation *op)
{
	BST* node = new BST;
	op->count(2);
	node->key = value;
	node->size = size;
	//node->left = NULL;
	//node->right = NULL;
	return node;
}

BST* createBST(int left, int right, int arr[],Operation *op)
{
	op->count();
	if (right < left)
		return NULL;
	op->count(2);
	int mid = (left + right) / 2;
	BST* node = createNode(arr[mid],right-left+1,op);
	op->count();
	node->left = createBST(left, mid - 1, arr,op);
	op->count();
	node->right = createBST(mid + 1, right, arr,op);
	return node;
}

BST* os_select(BST* root, int i,Operation *op)
{
	op->count();
	if (root == NULL)
		return NULL;	
	op->count(2);
	int r = (root->left ? root->left->size : 0) + 1; //r-rank - pozitia nodului curent in sirul sortat
	op->count();
	if (r == i)
		return root;
	op->count();
	if (i < r)
		return os_select(root->left, i, op);
	else
		return os_select(root->right, i - r,op);
}
BST* findMin(BST* root,Operation *op) //functie pt aflarea minimului intr-un arbore
{
	while (root->left != NULL)
	{
		op->count();
		root = root->left;
	}
	return root;
}

BST* deleteNode(BST* root, int key,Operation *op)
{	
	op->count();
	if (root == NULL)
		return NULL;
	op->count();
	if (key < root->key) //cautare nod unde se gaseste
		root->left = deleteNode(root->left, key,op);
	else
	{
		op->count();
		if (key > root->key)
			root->right = deleteNode(root->right, key, op);
		else
		{
			op->count();
			if (root->left == NULL) //caz1 - nodul nu are copil stanga
			{
				op->count();
				BST* temp = root->right;
				delete root;
				return temp;
			}
			op->count();
			if (root->right == NULL) //caz2 - nodul nu are copil dreapta
			{
				op->count();
				BST* temp = root->left;
				delete root;
				return temp;
			}
			BST* temp = findMin(root->right, op); //gasesc succesorul si copiez cheia lui in nod,dupa care delete succesor //caz3 - cu 2 copii
			op->count(2);
			root->key = temp->key;
			root->right = deleteNode(root->right, temp->key, op);
		}
	}
	op -> count(3);
	root->size = 1+ (root->left ? root->left->size : 0) + (root->right ? root->right->size : 0); //update size pentru partea modificata
	return root;
}

BST* os_delete(BST* root, int i,Operation *op)
{
	BST* node = os_select(root, i,op);
	op->count();
	if (node == NULL)
		return node;
	return deleteNode(root, node->key,op);
}

void pretty_print(BST* root, int tabs) 
{
	if (root == NULL)
		return;
	pretty_print(root->right, tabs + 1); //afisare subarbore drept
	for (int i = 0; i < tabs; i++)
		cout << "   ";
	cout << root->key << " (" << root->size << ")" << endl;
	pretty_print(root->left, tabs + 1);//afisare subarbore stang
}

void demo_func()
{
	int a[] = { 0,1,2,3,4,5,6,7,8,9,10 };
	int left = 0;
	int right = 10;
	Operation op = profiler.createOperation("cnt", right);
	BST* tree = createBST(left, right, a,&op);
	cout << "Arborele BST construit este:" << endl;
	pretty_print(tree, 0);

	cout << "\nSelectarea elementelor random:" << endl;
	for (int i = 1; i <= 10; i++)
	{
		int x = 1 + rand() % (11);
		BST* select = os_select(tree, x, &op);
		if(select != NULL)
			cout << "Elementul al "<<x<<"-lea este : " << select->key << endl;
	}
	
	cout << "\nStergerea unor elemente random:" << endl;
	srand(time(0));
	for (int i = 0; i <= 3; i++)
	{
		
		if (tree == NULL || tree->size == 0)
		{
			cout << "Arborele este gol." << endl;
			break;
		}
		int x =  rand() % (11) ;
		cout << "Stergere al " << x << "-lea cel mai mic element : " << endl;
		tree = os_delete(tree, x, &op);
		cout << "Arborele dupa stergere:" << endl;
		pretty_print(tree, 0);
		cout << "------------------------------" << endl;	
	}
}

void demo_analysis()
{
	int max_size = 10000;

	for (int i = 100; i < 10000; i += 100)
	{
		Operation op1 = profiler.createOperation("BuildTreeOperation", i);
		Operation op2 = profiler.createOperation("OsSelectOperation", i);
		Operation op3 = profiler.createOperation("OsDeleteOperation", i);
		for (int test = 0; test < 5; test++)
		{
			int* arr = new int[i];
			FillRandomArray(arr, i, 0, 15000, true, 1);
			BST* tree = createBST(0, i - 1, arr, &op1);
			for (int j = 0; j < i; j++)
			{
				int x = rand() % i;
				os_select(tree, x, &op2);
				os_delete(tree, x, &op3);
			}
			delete arr;
		}
	}
	profiler.divideValues("BuildTreeOperation", 5);
	profiler.divideValues("OsSelectOperation", 5);
	profiler.divideValues("OsDeleteOperation", 5);
	profiler.createGroup("OS", "BuildTreeOperation", "OsSelectOperation", "OsDeleteOperation");
	profiler.showReport();
}

int main()
{
	//demo_func();
	//demo_analysis();
	demoAVL();
	return 0;
}
