/*
* @author Breban Vasile , grupa 8
* Cerinte : 
*	Implementarea operației de inserare și căutare folosind structura de date 
*	cerută + demo (dimensiune 10)
*	Evaluarea operației de căutare pentru un singur factor de umplere 95%
*	Completarea evaluării pentru toți factorii de umplere
*	Implementare operației de ștergere într-o tabelă de dispersie, demo (dimensiune 10) și evaluarea operației de căutare după ștergerea unor elemente 
* 
*	Pasi de rezolvare:
*	-am implementat prima data functiile de baza pentru o tabela de dispersie : functia de hash,de insert,de cautare si de stergere
*	-am demonstrat corectitudinea functiilor prin apelul a 2 functii void in main
*	-am creat o functie eval_perform pentru operatia de cautare pe o tabela cu filling factor specific.Am generat un array de n elemente care mai apoi
*    l-am introdus in tabela de dispersie.
*	-pentru partea de evaluare,pentru 1500 de elemente am luat elemente deja existente in tabela,iar pentru restul de 1500 am generat o cheie care sa aiba o 
*	 valoare care nu se gaseste in tabela,avand valori intre 20000 si 30000 (la fillRandomArray eu am dat ca elementele sa fie intre 0 si 20000)
*	-am calculat si afisat termenii corespunzatori tabelului prezent in laborator
*   -iar pentru evaluarea performantei de cautare dupa stergere, am folosit o functie asemanatoare, doar am introdus functia de delete care sa stearga numarul 
*    necesar de termeni pt a se ajunge la filling factor de 0.8(de la 0.99)
*/

#include <iostream>
#include "Profiler.h"
#include <cstring>
#include <cstdlib>
using namespace std;

typedef struct {
	int id;
	int status;
	char name[30];
}HashTable;
enum{FREE, OCCUPIED};

const int  MAX_SIZE = 10007; //prime number
//node *newNode(int value)
//{
//	node* new_node = new node;
//	new_node->id = value;
//	new_node->next = NULL;
//	return new_node;
//}
int hash(int k, int n)
{
	return k % n;
}

int hashQuad(int k, int n, int i)
{
	return (abs(k%n) + i + (i * i)) % n; //abs pt ca am incercat si cu numere negative
}

void insert_key(HashTable *T,int key,int n,const char *name)
{
	int i = 0;
	int index = hashQuad(key, n, i);
	while (i < n)
	{
		if (T[index].status == FREE)
		{
			T[index].id = key;
			if(name != NULL)
				strcpy(T[index].name, name);
			T[index].status = OCCUPIED;
			return;
		}
		i++;
		index = index = hashQuad(key, n, i);
	}
	cout << "Tabela este plina! Nu se mai pot insera elemente."<<endl;
}

int search_key(HashTable* T, int key, int n,int *op_count)
{
	int i = 0;
	int index = hashQuad(key, n, i);
	*op_count = 0;
	while (i < n)
	{
		(*op_count)++;
		if (T[index].status == FREE)
			break; //ne oprim cand gasim un slot liber
		if (T[index].status == OCCUPIED && T[index].id == key)
		{
			//cout << "Cheia " << T[index].id << " gasita la pozitia " << index <<" cu numele "<<T[index].name<< endl;
			return index;
		}
		i++;
		index = hashQuad(key, n, i);
	}
	//cout << "Cheia nu a fost gasita." << endl;
	return -1; //not found
}

void delete_key(HashTable T[], int key, int n)
{
	int i = 0;
	int index = hashQuad(key, n, i);
	while (i < n)
	{
		if (T[index].status == FREE)
		{
			//cout << "Key not found." << endl;
			return;
		}
		if (T[index].status == OCCUPIED && T[index].id == key)
		{
			T[index].status = FREE;
			//cout << "Key " << key <<" deleted" << endl;
			return;
		}
		i++;
		index = hashQuad(key, n, i);
	}
	//cout << "Key not found for delete." << endl;
}

void eval_perform_search(double alpha)
{
	int x = MAX_SIZE;
	int n = (int)(alpha * x); //nr de elemente de inserat calculat dupa filling factor
	HashTable T[MAX_SIZE];
	for (int i = 0; i < MAX_SIZE; i++)
	{
		T[i].status = FREE;
	}
	int* array = new int[n];
	FillRandomArray(array, n, 0,20000, false , 0);
	
	for (int i = 0; i < n; i++)
	{
		insert_key(T, array[i], MAX_SIZE, NULL);
	}
	int found_cnt = 0;
	int total_op_found = 0;
	int total_op_notf = 0;
	int max_op_found = 0;
	int max_op_notf = 0;

	for (int i = 0; i < 3000; i++)
	{
		int key;
		if (i < 1500)
		{
			key = array[rand() % n]; //selecteaza random o cheie existenta in hash table
		}
		else
		{
			key = rand() % 10000 + 20000; //genereaza (intre 20000 si 30000) in mod unic o cheie care cel mai probabil nu se afla in hash table
		}

		int op_count = 0;
		int result = search_key(T, key, n, &op_count);
		if (result != -1) //daca gasim cheia
		{
			found_cnt++;
			total_op_found += op_count;
			max_op_found = max(max_op_found, op_count);
		}
		else
		{
			total_op_notf += op_count;
			max_op_notf = max(max_op_notf, op_count);
		}
	}
	double avg_effort_found, avg_effort_notf;
	if (found_cnt > 0){
		avg_effort_found = (double)total_op_found / found_cnt;
	}
	else {
		avg_effort_found = 0;
	}
	if (3000 - found_cnt > 0){
		avg_effort_notf = (double)(total_op_notf) / (3000 - found_cnt);
	}
	else {
		avg_effort_notf = 0;
	}
		//double avg_effort_found = (found_cnt > 0) ? (double)total_op_found / found_cnt : 0;
		//double avg_effort_notf = (3000 - found_cnt > 0) ? (double)(total_op_notf) / (3000 - found_cnt) : 0;
		cout << alpha << "             " << avg_effort_found << "             " << max_op_found << "                " << avg_effort_notf 
		<< "                " << max_op_notf << endl;
		delete[] array;//free memory
}

void eval_perform_with_delete(double alpha, double target)
{
	int x = MAX_SIZE;
	int n = (int)(x * alpha);
	HashTable T[MAX_SIZE];
	for (int i = 0; i < MAX_SIZE; i++)
	{
		T[i].status = FREE;
	}
	int* array = new int[n];
	FillRandomArray(array, n, 0, 20000, false, 0);

	for (int i = 0; i < n; i++)
	{
		insert_key(T, array[i], MAX_SIZE, NULL);
	}
	int target_n = (int)(target * x);
	int delete_cnt = n - target_n; //numarul de elemente ce vor fi sterse
	for (int i = 0; i < delete_cnt; i++)
	{
		delete_key(T, array[i], MAX_SIZE);
	}
	int found_cnt = 0;
	int total_op_found = 0;
	int total_op_notf = 0;
	int max_op_found = 0;
	int max_op_notf = 0;

	for (int i = 0; i < 3000; i++)
	{
		int key;
		if (i < 1500)
		{
			key = array[rand() % n]; //selecteaza random o cheie existenta in hash table
		}
		else
		{
			key = rand() % 10000 + 20000; //genereaza (intre 20000 si 30000) in mod unic o cheie care cel mai probabil nu se afla in hash table
		}

		int op_count = 0;
		int result = search_key(T, key, n, &op_count);
		if (result != -1) //daca gasim cheia
		{
			found_cnt++;
			total_op_found += op_count;
			max_op_found = max(max_op_found, op_count);
		}
		else
		{
			total_op_notf += op_count;
			max_op_notf = max(max_op_notf, op_count);
		}
	}
	double avg_effort_found, avg_effort_notf;
	if (found_cnt > 0) {
		avg_effort_found = (double)total_op_found / found_cnt;
	}
	else {
		avg_effort_found = 0;
	}
	if (3000 - found_cnt > 0) {
		avg_effort_notf = (double)(total_op_notf) / (3000 - found_cnt);
	}
	else {
		avg_effort_notf = 0;
	}
	//double avg_effort_found = (found_cnt > 0) ? (double)total_op_found / found_cnt : 0;
	//double avg_effort_notf = (3000 - found_cnt > 0) ? (double)(total_op_notf) / (3000 - found_cnt) : 0;
	cout << target << "             " << avg_effort_found << "             " << max_op_found << "                " << avg_effort_notf
		<< "                " << max_op_notf << endl;
	delete[] array;//free memory
}

void demo1()
{
	vector<double> alpha = { 0.80,0.85,0.90,0.95,0.99 };
	cout << "FillingFactor|" << "Avg.effort(found)|" << "Max effort(found)|" << "Avg.effort(not_found)|" << "Max.effort(not_found)" << endl;
	cout << "---------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < 5; i++)
	{
		for (int test = 0; test < 5; test++)
		{
			eval_perform_search(alpha[i]);
		}
		cout << "---------------------------------------------------------------------------------------------" << endl;
	}
}

void demo2()
{
	double alpha = 0.99;
	double target = 0.8;
	cout << "Search evaluation after delete" << endl;
	cout << "FillingFactor|" << "Avg.effort(found)|" << "Max effort(found)|" << "Avg.effort(not_found)|" << "Max.effort(not_found)" << endl;
	cout << "---------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < 5; i++)
	{
		eval_perform_with_delete(alpha, target);
	}
}

void demo_corectitudine() {
	int n = 10;
	HashTable T[10];
	for (int i = 0; i < n; i++)
	{
		T[i].status = FREE;  //initializare tabela goala
	}
	int op = 0;
	insert_key(T, 5, n, "Gheorghe");
	insert_key(T, 7, n, "Vasile");
	insert_key(T, 13, n, "Ion");
	insert_key(T, 26, n, "Andreea");
	insert_key(T, 24, n, "George");
	insert_key(T, 11, n, "Georgel");
	insert_key(T, 12, n, "Georgeta");
	insert_key(T, 10, n, "Andrei");
	insert_key(T, 18, n, "Emanuel");
	insert_key(T, 29, n, "Teodora");

	search_key(T, 5, n, &op);
	search_key(T, 7, n, &op);
	search_key(T, 13, n, &op);
	search_key(T, 35, n, &op); //nu e gasit
	search_key(T, 26, n, &op);
	search_key(T, 24, n, &op);
	search_key(T, 11, n, &op);
	search_key(T, 12, n, &op);
	search_key(T, 10, n, &op);
	search_key(T, 18, n, &op);
	search_key(T, 29, n, &op);

	insert_key(T, 21, n, "Marius"); //tabela este plina
	search_key(T, 21, n, &op); //nu e gasit

	delete_key(T, 29, n);
	search_key(T, 29, n, &op);
}

void afisare(HashTable T[], int size)
{
	cout << "Tabela de dispersie este:" << endl;
	for (int i = 0; i < size; i++)
	{
		if (T[i].status == OCCUPIED)
			cout << "Elementul pe pozitia "<<i<<" este "<<T[i].id << endl;
		else
			cout << "Nu exista element pe pozitia " << i<<endl;
	}
	cout << endl;
}

void demo_corectitudine2()
{
	int m = 10;
	HashTable T[10];
	for (int i = 0; i < m; i++)
	{
		T[i].status = FREE;
	}
	int values[] = { 2,19,32,101,14 };
	int x = sizeof(values) / sizeof(int);
	for (int i = 0; i < x; i++)
	{
		insert_key(T, values[i], m, NULL);
	}
	afisare(T, m);
	int values2[] = { 1,19,31,101,15 };
	int op = 0;
	for (int i = 0; i < x; i++)
	{
		int search = search_key(T, values2[i], m, &op);
		if (search != -1)
		{
			cout << "Cheia "<<values2[i]<<" a fost gasita"<<endl;
		}
		else
		{
			cout << "Cheia " << values2[i] << " nu a fost gasita." << endl;
		}
	}
	delete_key(T, 32, m);
	int y = search_key(T, 32, m, &op);
	if (y == -1)
	{
		cout << "Cheia 32 nu a fost gasita datorita stergerii";
	}
}

int main()
{
	//demo_corectitudine();
	//demo_corectitudine2();
	demo1(); //pt search
	//demo2(); //pentru op de delete-search
	return 0;
}