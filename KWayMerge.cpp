/*
*@Author Breban Vasile,grupa8
* 
* Subiect: Interclasarea eficientă a k șiruri ordonate 
* Functiile sink si makeHeap sunt din laboratoarele trecute si ne ajuta sa mentinem proprietatea de min-heap
* Functia principala mergeKWay realizeaza interclasarea a k siruri de dimensiune n,sirurile fiind salvate intr-o lista de liste iar rezultatul
* interclasarii este pus intr-o coada
* Functia generateLists imi genereaza k liste de dimeniune n,fiecare sir avand n/k in medie elemente(folosesc functia fillRandomArray pentru a 
* genera un sir ordonat crescator,care mai apoi il introduc in lista de liste)
* 
* Analiza complexitate:
*	-pentru cazul in care k=5,10,100 iar n>=100 si n<10000 se optine o complexitate O(nlogk), n fiind numarul de elemente total al listelor 
*    iar k fiind numarul de liste -- cel mai bine se vede pentru k=100 graficul
*	-pentru cazul in care n=10000 si k variaza intre 10 si 500 se obtine din grafice o complexitate ce arata a O(logk) dar in practiac este O(n*klogk)
*/

#include <iostream>
#include "Profiler.h"
#include <queue>
#include <vector>
using namespace std;
Profiler profiler;
#define STEP_SIZE 100
#define MAX_SIZE 10000
#define NR_TESTS 5
struct node
{
	int key;
	struct node* next;
};
node *h[505]; //lista de liste
queue <int> sol;
node* createNode(int value)
{
	node* new_node = new node;
	new_node->key = value;
	new_node->next = NULL;
	return new_node;
}

void sink(node* h[],int index, int k,Operation *assg,Operation *cmp)
{
	while (2 * index <= k) {
		int min = 2 * index; //min este index
		//cmp->count(1);
		if (min + 1 <= k && h[min + 1]->key < h[min]->key)
			min++;
		//cmp->count(1);
		if (h[min]->key < h[index]->key)
		{
			//assg->count(3);
			swap(h[min], h[index]);//swap intre array-uri
			index = min;
		}
		else
			break;
	}
}
void makeHeap(node* h[], int k, Operation* assg, Operation* cmp)
{
	for (int i = k / 2; i >= 1; i--)
	{
		sink(h, i, k,assg,cmp);
	}
}
void mergeKWay(node* h[], int k, Operation* assg, Operation* cmp)
{
	makeHeap(h, k,assg,cmp);
	int heap_size = k;
	while (heap_size > 0)
	{
		//assg->count(1);
		sol.push(h[1]->key);
		//cmp->count(1);
		if (h[1]->next != NULL)
		{
			//assg->count(2);
			node* aux = h[1];
			h[1] = h[1]->next;
			free(aux);
		}
		else
		{
			//assg->count(2);
			node* aux = h[1];
			h[1] = h[heap_size];
			free(aux);
			heap_size--;
		}
		sink(h, 1, heap_size,assg,cmp);
	}
}
void generateLists(int k, int n)
{
	vector<int> list_sizes(k, n / k);//vector de size k,umplut cu valoarea n/k
	int remainder = n % k;
	for (int i = 0; i < remainder; i++)
	{
		list_sizes[i]++;
	}
	for (int i = 0; i < k; i++)
	{
		int size = list_sizes[i];
		int* values = new int[size];
		FillRandomArray(values, size, -5000,5000 , false, 1); //generez un sir sortat crescator
		node* head = createNode(values[0]); 
		h[i + 1] = head; //inserez elementele sirului in lista de liste
		for (int j = 1; j < size; j++)
		{
			head->next = createNode(values[j]);
			head = head->next;
		}
	}
}
//de generat n/k elemente pe lista
void demoGenerate()
{
	int k = 4;
	int n = 20;
	Operation assignments = profiler.createOperation("Nume", k);
	Operation comparisons = profiler.createOperation("Nume2", k);
	generateLists(k, n);
	cout << "Generated sorted lists: "<<endl;
	for (int i = 1; i <= k; i++)
	{
		node* current = h[i];
		cout << "List " << i <<": ";
		while (current != NULL) //afisare sir din lista de liste
		{
			cout << current->key << " ";
			current = current->next;
		}
		cout << endl;
	}
	mergeKWay(h, k, &assignments, &comparisons);

	cout << "Merged sorted result:" << endl; //afisez interclasarea sirurilor stocate in coada
	while (!sol.empty()) {
		cout << sol.front() << " ";
		sol.pop();
	}
	cout << endl;
}

void demo1() {
	int k_values[] = { 5,10,100};
	int k = k_values[0];
		for (int n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
		{
			Operation assg = profiler.createOperation("AssignmentsNumber_K5", n);
			Operation cmp = profiler.createOperation("ComparisonsNumber_K5", n);
			for (int test = 0; test < NR_TESTS; test++)
			{
				generateLists(k, n);
				mergeKWay(h, k, &assg, &cmp);
			}
		}
	profiler.divideValues("AssignmentsNumber_K5", NR_TESTS);
	profiler.divideValues("ComparisonsNumber_K5", NR_TESTS);
	profiler.addSeries("TotalSum_K5", "ComparisonsNumber_K5", "AssignmentsNumber_K5");

	k = k_values[1];
	for (int n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		Operation assg2 = profiler.createOperation("AssignmentsNumber_K10", n);
		Operation cmp2 = profiler.createOperation("ComparisonsNumber_K10", n);
		for (int test = 0; test < NR_TESTS; test++)
		{
			generateLists(k, n);
			mergeKWay(h, k, &assg2, &cmp2);
		}
	}
	profiler.divideValues("AssignmentsNumber_K10", NR_TESTS);
	profiler.divideValues("ComparisonsNumber_K10", NR_TESTS);
	profiler.addSeries("TotalSum_K10", "ComparisonsNumber_K10", "AssignmentsNumber_K10");

	k = k_values[2];
	for (int n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		Operation assg3 = profiler.createOperation("AssignmentsNumber_K100", n);
		Operation cmp3 = profiler.createOperation("ComparisonsNumber_K100", n);
		for (int test = 0; test < NR_TESTS; test++)
		{
			generateLists(k, n);
			mergeKWay(h, k, &assg3, &cmp3);
		}
	}
	profiler.divideValues("AssignmentsNumber_K100", NR_TESTS);
	profiler.divideValues("ComparisonsNumber_K100", NR_TESTS);
	profiler.addSeries("TotalSum_K100", "ComparisonsNumber_K100", "AssignmentsNumber_K100");

	profiler.createGroup("TotalComparisons", "ComparisonsNumber_K5", "ComparisonsNumber_K10", "ComparisonsNumber_K100");
	profiler.createGroup("TotalAssignments", "AssignmentsNumber_K5", "AssignmentsNumber_K10", "AssignmentsNumber_K100");
	profiler.createGroup("TotalSum", "TotalSum_K5", "TotalSum_K10", "TotalSum_K100");
	profiler.showReport();
 }

void demo2()
{
	int n = MAX_SIZE;
	for (int k = 10; k < 500; k += 10)
	{
		Operation assg = profiler.createOperation("AssignmentsNumber", k);
		Operation cmp = profiler.createOperation("ComparisonsNumber", k);
		for (int test = 0; test < NR_TESTS; test++)
		{
			generateLists(k, n);
			mergeKWay(h, k, &assg, &cmp);
		}
	}
	profiler.divideValues("AssignmentsNumber", NR_TESTS);
	profiler.divideValues("ComparisonsNumber", NR_TESTS);
	profiler.addSeries("TotalSum", "ComparisonsNumber", "AssignmentsNumber");
	profiler.showReport();
}
void demo() {
	int k = 4;
	int n = 21;

	generateLists(k, n);
	for (int i = 1; i <= k; i++)
	{
		node* current = h[i];
		cout << "List " << i << ": ";
		while (current != NULL) //afisare sir din lista de liste
		{
			cout << current->key << " ";
			current = current->next;
		}
		cout << endl;
	}
	mergeKWay(h, k, NULL, NULL);
	cout << "Merged sorted result:" << endl; //afisez interclasarea sirurilor stocate in coada
	while (!sol.empty()) {
		cout << sol.front() << " ";
		sol.pop();
	}
	cout << endl;
	
}
int main()
{
	//demoGenerate();
	//demo1();
	//demo2();
	demo();
	return 0;
}