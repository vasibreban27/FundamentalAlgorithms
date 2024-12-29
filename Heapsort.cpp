/*
* @author Breban Vasile
* @group 30228

Probleme rezolvate :
1.Analiza comparativă a unuia din algoritmii de sortare din L1 (la alegere) în
versiune iterativă vs recursivă. Analiza se va efectua atât din perspectiva numărului
de operații, cât și a timpului de rulare
2.Implementarea metodei bottom-up de construire a unui heap
3.Implementarea metodei top-down de construire a unui heap
4.Analiza comparativă a celor două metode de construire în cazul mediu statistic 
5.Analiza comparativă a metodelor de construcție în cazul defavorabil
6.Implementarea și exemplificarea corectitudinii algoritmului heapsort

*  Metoda bottom-up(sink) are o complexitate O(n) si este mai eficienta decat metoda top-down(swim) care are o complexitate O(nlogn)
* Pe caz mediu statistic,algoritmul sink este mai eficient atat dpdv asignari cat si comparari decat swim
* Pe caz worst statistic,algoritmul sink este mai eficient atat dpdv asignari cat si comparari decat swim
* Algoritmul heapsort sorteaza crescator un vector min-heap,complexitatea totala fiind O(nlogn)
*/
#include <iostream>
#include "Profiler.h"
using namespace std;
Profiler profiler;
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void swim(int* a,int n, int index)//top-down
{
	Operation assg = profiler.createOperation("SwimAssignments", n);
	Operation cmps = profiler.createOperation("SwimComp", n);
	while (index > 1 && a[index] < a[index / 2])
	{
		cmps.count();
		assg.count(3);
		swap(a[index], a[index / 2]);
		index /= 2;
	}
}

void made_heap_swim(int* a, int n) //heapify-up,complexitate O(nlogn)
{
	for (int i = 2; i <= n; i++)
	{
		swim(a, n, i);
	}
}

void sink(int* a, int n, int index) //bottom-up
{
	Operation assg = profiler.createOperation("SinkAssignments", n);
	Operation cmps = profiler.createOperation("SinkComp", n);
	while (2*index < n) //porneste de la primul nod care are copii
	{
		int ind_child = 2 * index;
		if (ind_child + 1 <= n && a[ind_child + 1] < a[ind_child])
		{
			cmps.count();
			ind_child++;
		}
		if (a[index] > a[ind_child])
		{
			cmps.count();
			assg.count(3);
			swap(a[index], a[ind_child]);
			index = ind_child;
		}
		else
			break;
	}
}

void made_heap_sink(int* a, int n)//heapify-down, O(n) complexitate
{
	for (int i = n/2; i >=1; i--)
	{
		sink(a, n, i);
	}
}

int* heapsort(int* a, int n)
{
	int* b = new int[n];//alocare dinamica
	int index = 1;
	while (n > 1) //posibil >= 1 
	{
		b[index] = a[1];
		index++;
		swap(a[1], a[n]);
		n--;
		sink(a, n, 1);
	}
	b[index] = a[1];
	return b;
}
void afisare_vector(int* a, int n)
{
	for (int i = 1; i <= n; i++)
		printf("%d ", a[i]);
	printf("\n");
}
void insertionSort(int* arr, int n) {
	Operation as_insertion = profiler.createOperation("InsertionSortAssignments", n);
	Operation comp_insertion = profiler.createOperation("InsertionSortComp", n);

	for (int i = 1; i < n; i++) {
		as_insertion.count();
		int aux = arr[i];
		int j = i - 1;

		while (comp_insertion.count(), arr[j] > aux && j >= 0) {
			as_insertion.count();
			arr[j + 1] = arr[j];
			j--;
		}
		as_insertion.count();
		arr[j + 1] = aux;
	}
}

void recursiveInsertionSort(int* arr, int n,Operation *recursive_counter)
{
	if (recursive_counter == NULL)
		return;
	if (n <= 1)
		return;
	recursiveInsertionSort(arr, n - 1,recursive_counter);
	recursive_counter->count();
	int aux = arr[n - 1];
	int j = n - 2;
	while (j >= 0 && (recursive_counter->count(), arr[j] > aux)) {
		recursive_counter->count();
		arr[j + 1] = arr[j];
		j--;
	}
	recursive_counter->count();
	arr[j + 1] = aux;
}
void demo_insertionSort()
{
	int b[] = { 0,14,2,1,6,9 };
	int m = sizeof(b) / sizeof(int);
	insertionSort(b, m);
	printf("Vectorul sortat prin insertion sort este: ");
	for (int i = 0; i < m; i++)
	{
		printf("%d ", b[i]);
	}
	printf("\n");
}

void demo_recursiveSort()
{
	int a[] = { 0,7,2,5,4 };
	int n = sizeof(a) / sizeof(int);
	Operation op = profiler.createOperation("NumberRecOp",n);
	recursiveInsertionSort(a, n,&op); 
	printf("Vectorul sortat prin insertion sort recursiv este: ");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
}

void demo_sink_swim()
{
	int d[] = { 0,7,2,5,4,10 };
	int size = sizeof(d) / sizeof(int) - 1; //primul element in d[] nu se se ia in considerare,algoritmii functionand cu indexare de la 1 
	made_heap_swim(d, size);
	printf("Afisare vector made_heap_swim: ");
	afisare_vector(d, size); //cu i=1 <= n

	int d2[] = { 0,7,2,5,4,10 };
	int size2 = sizeof(d2) / sizeof(int) - 1;
	made_heap_sink(d2, size2);
	printf("Afisare vector made_heap_sink: ");
	afisare_vector(d2, size2);
}

void demo_heapsort()
{
	int x[] = { 0,1,3,2,7,5 }; //min heap -> parintele mai mic sau egal cu copii lui
	int size3 = 5;
	printf("Vectorul sortat cu heapsort: ");
	int* arr = heapsort(x, size3);
	for (int i = 1; i <= size3; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void perf()
{
	int c[MAX_SIZE];
	int i;
	
	for (i = STEP_SIZE; i < MAX_SIZE; i += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			Operation recursive_sort = profiler.createOperation("RecursivNumberOperations", i);
			FillRandomArray(c, i, -5000, 5000, false, 0);
			profiler.startTimer("TimerInsertionSort", i);
			for (int j = 0; j < 1000; j++)
			{
				insertionSort(c, i);
			}
			profiler.stopTimer("TimerInsertionSort", i);

			FillRandomArray(c, i, -5000, 5000, false, 0);
			profiler.startTimer("TimerRecursiveInsertionSort", i);
			for (int j = 0; j < 1000; j++)
			{
				recursiveInsertionSort(c, i,&recursive_sort);
			}
			profiler.stopTimer("TimerRecursiveInsertionSort", i);
		}
	}
	profiler.createGroup("TimerTotal", "TimerRecursiveInsertionSort", "TimerInsertionSort");
	for (i = STEP_SIZE; i < MAX_SIZE; i += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++) {
			Operation recursive_sort = profiler.createOperation("RecursiveSort", i);
			FillRandomArray(c, i, -5000, 5000, false, 0);
			insertionSort(c, i);
			FillRandomArray(c, i, -5000, 5000, false, 0);
			recursiveInsertionSort(c, i, &recursive_sort);
		}
	}
	profiler.divideValues("InsertionSortComp", NR_TESTS);
	profiler.divideValues("InsertionSortAssignments", NR_TESTS);
	//profiler.divideValues("RecursiveInsertionSortComp", NR_TESTS);
	//profiler.divideValues("RecursiveInsertionSortAssignments", NR_TESTS);
	profiler.addSeries("InsertionSortIterativ", "InsertionSortComp", "InsertionSortAssignments");
	profiler.createGroup("InsertionSortTotal", "InsertionSortIterativ", "RecursiveSort");
	//profiler.createGroup("ARecursiveInsertionSort", "RecursiveInsertionSortComp", "RecursiveInsertionSortAssignments");
	
	for ( i = STEP_SIZE; i < MAX_SIZE; i += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++) {
			FillRandomArray(c, i, -5000, 5000, false, 0);
			made_heap_sink(c, i);
			FillRandomArray(c, i, -5000, 5000, false, 0);
			made_heap_swim(c, i);
		}
	}
	profiler.divideValues("SinkComp", NR_TESTS);
	profiler.divideValues("SinkAssignments", NR_TESTS);
	profiler.divideValues("SwimComp", NR_TESTS);
	profiler.divideValues("SwimAssignments", NR_TESTS);
	profiler.createGroup("CompAverage", "SinkComp", "SwimComp");
	profiler.createGroup("AssignmentsAverage", "SinkAssignments", "SwimAssignments");
	profiler.addSeries("SwimAverage", "SwimAssignments", "SwimComp");
	profiler.addSeries("SinkAverage", "SinkAssignments", "SinkComp");
	profiler.createGroup("OverallAverage", "SwimAverage", "SinkAverage");

	profiler.reset("SwimAssignments");
	profiler.reset("SwimComp");
	profiler.reset("SinkAssignments");
	profiler.reset("SinkComp");

	for (i = STEP_SIZE; i < MAX_SIZE; i += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++) {
			FillRandomArray(c, i, -5000, 5000, false, 2);
			made_heap_sink(c, i);
			FillRandomArray(c, i, -5000, 5000, false, 2);
			made_heap_swim(c, i);
		}
	}
	profiler.divideValues("SinkComp", NR_TESTS);
	profiler.divideValues("SinkAssignments", NR_TESTS);
	profiler.divideValues("SwimComp", NR_TESTS);
	profiler.divideValues("SwimAssignments", NR_TESTS);
	profiler.createGroup("CompWORST", "SinkComp", "SwimComp");
	profiler.createGroup("AssignmentsWORST", "SinkAssignments", "SwimAssignments");
	profiler.addSeries("SwimWORST", "SwimAssignments", "SwimComp");
	profiler.addSeries("SinkWORST", "SinkAssignments", "SinkComp");
	profiler.createGroup("OverallWORST", "SwimWORST", "SinkWORST");

	profiler.showReport();
}
int main()
{
	//verificare corectitudine cei 2 alg. de sortare pe vectori de dimensiune mica
	demo_insertionSort();
	demo_recursiveSort();
	
	//verificare functionalitate algoritmi de construire heap bottom up si top down
	demo_sink_swim();
	//verificare functionalitate heapsort
	demo_heapsort();
	//afisare grafice timere,insertion iterativ si recursiv,sink si swim average si worst case
	perf();
	return 0;
}