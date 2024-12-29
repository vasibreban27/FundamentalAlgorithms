/*
* @author Breban Vasile, group 8
*  Tema 6 - Arbori multicai
* 
*  1.Implementare a parcurgerii iterative și recursive a unui arbore binar în O(n) și cu 
*   memorie aditională constantă (2p)
* -aici am ales parcurgerea inorder (left,root,right) pe care am scris-o atat recursiv cat si iterativ
*  2.Analiza comparativă a parcurgerii iterative și recursive din perspectiva numărului de operatii(2p)
* - functia demo_analysis face analiza celor 2 parcurgeri,ambele fiind in O(n) liniar, radacina fiind aleasa in mod aleatoriu,
*	dimenisunea tabloului pornind de la 100 -> 10000
* 3.Implementarea corectă la pretty-print la R1 
* 4.Implementarea corectă la T1 și pretty-print la R2 (1p) + T1 în timp liniar
* 5.Implementarea corectă la T2 și pretty-print la R3 (2p) + T2  în timp liniar
*  
*  -pentru transformarea 1 adica R1->R2,algoritmul parcurge cu un for vectorul de parinti si stocam nodurile in R2_const pt complexitate liniara la spatiu
*      --complexitate O(n)
*  -pentru transformarea 2 adica R2->R3,algoritmul parcurge cu un for lista de copii si cream binary tree-ul
*     --complexitate O(n)
* 
* --structuri folosite : 
*		-node2 ce contine cheia si lista de copii a fiecarui nod
*		-node3 ce contine cheia si pointeri catre fratele stang si drept al unui nod
*/


#include <iostream>
#include <list>
#include <vector>
#include "Profiler.h"
using namespace std;
Profiler profiler;
struct node2
{
	int key;
	list<node2*> sons;
};
node2* R2, *R2_const[500]; //r2 const pt linear time

struct node3
{
	int key;
	node3* left, *right;
};
node3 R3;
node3* create_node(int key)
{
	node3* newNode = new node3;
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}
void inorderRec(node3* root,Operation *counter)
{
	if (root != NULL)
	{
		inorderRec(root->left,counter);
		counter->count();
		//cout << root->key << " ";
		inorderRec(root->right,counter);
	}
}

void inorderIterativ(node3* root, Operation* counter)
{
	node3* current = root;
	while (current != NULL)
	{
		if (current->left == NULL) //daca nu am copil stanga,merg in dreapta
		{
			counter->count();
			//cout << current->key << " ";
			current = current->right;
		}
		else
		{
			node3* pred = current->left;  
			while (pred->right != NULL && pred->right != current) //aflam predecesorul nodului curent
			{
				pred = pred->right;
			}
			if (pred->right == NULL) //daca nu avem pred,creeam o conexiune temporara
			{
				pred->right = current;
				current = current->left;
			}
			else
			{
				pred->right = NULL; //daca e deja conectat,subarborele stang este finalizat
				counter->count();
				//cout << current->key << " ";
				current = current->right;
			}
		}
	}
}

node3* constructBinaryTree(int array[], int size,int index)
{
	if (size == 0)
		return NULL;
	//int index = rand() % size;  //am incercat sa generez o pozitie random pentru radacina dar am facut altfel
	//node3* root = new node3{ array[index], NULL, NULL };
	node3** nodes = new node3 * [size];
	for (int i = 0; i < size; ++i) {
		nodes[i] = new node3{ array[i], NULL, NULL };
	}
	for (int i = 0; i < size; ++i) {
		int left = 2 * i + 1; // index of left child
		int right = 2 * i + 2; // index of right child
		if (left < size) {
			nodes[i]->left = nodes[left];
		}
		if (right < size) {
			nodes[i]->right = nodes[right];
		}
	}
	node3* root = nodes[0];
	//if (index * 2 + 1 < size)   
	//{
	//	root->left = nodes[index * 2 + 1];
	//}
	//else
	//{
	//	root->left = NULL;
	//}
	//if (index * 2 + 2 < size)
	//{
	//	root->right = nodes[index * 2 + 2];
	//}
	//else
	//{
	//	root->right = NULL;
	//}
	delete[] nodes;         
	return root;
}
void pretty_printR1(int parent[], int size,int tabs,int index_parent)
{
	for (int i = 0; i<tabs; i++)
	{
		cout << " ";
	}
	cout << index_parent<<endl;
	for (int i = 1; i <= size; i++) //nu avem 0 in arbore,pt indexare de la 0 i=0,i<n
	{
		if (parent[i] == index_parent)
		{
			pretty_printR1(parent, size, tabs + 1, i);
		}
	}
}

void pretty_printR2(node2 *R2,int tabs)
{
	for (int i = 0; i < tabs; i++)
	{
		cout << " ";
	}
	cout << R2->key << endl;
	for (auto it : R2->sons)
	{
		pretty_printR2(it, tabs + 1);
	}
}

void pretty_printR3(node3* R3, int tabs)
{
	if (R3 != NULL)
	{
		for (int i = 0; i < tabs; i++)
		{
			cout << " ";
		}
		cout << R3->key << endl;
		pretty_printR3(R3->left, tabs + 1);
		pretty_printR3(R3->right, tabs);
	}
}

node2* transformation1(int parent[], int size) {
	
	for (int i = 1; i <= size; i++) {
		R2_const[i] = new node2;  // alocare memorie pt fiecare nod
		R2_const[i]->key = i;    // setam cheia
		R2_const[i]->sons.clear(); //lista de copii goala
	}

	node2* root = NULL;
	for (int i = 1; i <= size; i++) {
		if (parent[i] == -1) {
			root = R2_const[i]; //cautam radacina si o salvam
		}
		else {
			// Add the current node to the sons of its parent
			//adaugam nodul curent la copiii parintelui
			R2_const[parent[i]]->sons.push_back(R2_const[i]);
		}
	}
	R2 = root; // setam R2 ca radacina globala a multi-way tree-ului
	return root;
}

node3* transformation2(node2* root) {
	if (root == NULL) 
		return NULL;

	node3* binaryRoot = create_node(root->key); //radacina noului arbore
	node3* previous = NULL; //pentru a tine urma ultimului nod inserat
	for (node2* child : root->sons) {
		node3* binaryChild = transformation2(child); // conversie a copiilor
		if (binaryRoot->left == NULL) {
			binaryRoot->left = binaryChild; // primul copil devine copilul din stanga
		}
		else {
			if (previous != NULL) {
				previous->right = binaryChild; //legam fratele ca fiind fratele din dreapta
			}
		}
		previous = binaryChild; //updatam ultimul nod inserat
	}
	return binaryRoot;
}

void test_construct()
{
	int values[] = { 5, 100, 17, 49, 51,10,32 };
	int size = 7;
	Operation op = profiler.createOperation("Count", size);
	node3* root = constructBinaryTree(values, size,0);
	inorderRec(root,&op);
}
void demo()
{
	int parent[] = { -100,2, 7, 5, 2, 7, 7, -1, 5, 2 }; // -1 e radacina
	int size = 9;
	cout << "Parent Tree Representation (R1):" << endl;
	pretty_printR1(parent, size, 0, 7);

	// Transformarea T1: R1 -> R2
	node2* R2 = transformation1(parent, size);
	cout << "\nMulti-way Tree Representation (R2):" << endl;
	pretty_printR2(R2, 0);

	// Transformarea T2: R2 -> R3 
	node3* R3 = transformation2(R2);
	cout << "\nBinary Tree Representation (R3):" << endl;
	pretty_printR3(R3, 0);
}

void demo_parcurgeri()
{
	node3* root = create_node(1);
	root->left = create_node(2);
	root->right = create_node(3);
	root->left->left = create_node(4);
	root->left->right = create_node(5);

	Operation op = profiler.createOperation("Count", 5);

	cout << "Recursive In-Order Traversal: ";
	inorderRec(root,&op);
	cout << endl;

	cout << "Iterative In-Order Traversal: ";
	inorderIterativ(root,&op);
	cout << endl;
}

void demo_analysis()
{
	for (int size = 100; size < 10000; size += 100)
	{
		int* arr = new int[size];
		FillRandomArray(arr, size, -5000, 5000, false, 0);
		int index = rand() % size; //aici ii dau radacina sa fie random din array
		node3* root = constructBinaryTree(arr, size,index);

		Operation cnt1 = profiler.createOperation("IterativeCounter", size);
		Operation cnt2 = profiler.createOperation("RecursiveCounter", size);

		inorderIterativ(root, &cnt1);
		inorderRec(root, &cnt2);
		delete[] arr;
	}
	profiler.showReport();
}
int main()
{
	demo(); //vizualizarea pretty-printurilor si efectuarea transformarilor
	//demo_parcurgeri(); //trebuie decomentate liniile  de cout din parcurgeri
	//test_construct();   //trebuie decomentate liniile de cout din inorder recursiv
	demo_analysis();   //trebuie comentate liniile de cout din cele 2 parcurgeri :)
	return 0;
}