/*
* @author Breban Vasile ,grupa 8
* Cerinte de implementat: QuickSort: implementare , QuickSort: evaluare în caz mediu statistic, favorabil si defavorabil,
* QuickSort și HeapSort: analiză comparativă a cazului mediu statistic , Implementarea hibridizării quicksort-ului
* Determinare a unui prag optim în hibridizare + motivație (grafice/măsuratori),Analiză comparativă (între quicksort și quicksort hibridizat) din punct de vedere a 
numărului de operații și a timpului efectiv de execuție,Bonus: QuickSelect - Randomized-Select 
*
*	Quicksort - sortarea unui sir prin partitionare(alegera pivotului pe ultima pozitie din sir)
* -pe caz average,complexitatea este O(nlogn),la fel si pe cazul best 
* -pe caz worst,complexitatea este O(n^2),am generat un sir descrescator cu pivotul pe ultima pozitie
* 
*	Quicksort vs Heapsort - pe caz mediu statistic,heapsort este mai eficient datorita numarului mai mici de operatii pe care le face,
* insa in practica quicksort este un algoritm mai rapid si este preferat
*	Hibridizare quicksort - daca array-ul dat are un numar de 10-20 de elemente,se foloseste insertion sort(este eficient pentru array-uri de dimeniuni mici)
* pentru sortarea array-ului
*	Pragul optim pentru quicksort hibridizat este de 10-15 elemente(uneori chiar si 20) din graficul obtinut
*	 Quicksort vs hibrid Quicksort - ambele au complexitate O(nlogn) iar varianta hibrida este mai eficienta dpdv al numarului de operatii,
* cat si a timpului de executie
*	QuickSelect - Randomized-Select - am creat o functie ce alege random pivotul in intervalul (0,n-1),iar quickselect apeleaza aceasta functie
* si returneaza elementul aflat pe pozitia k din sir-ul sortat
*/

#include <iostream>
#include <cstdlib>
#include "Profiler.h"
Profiler profiler("Quicksort");
using namespace std;
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

int partition(int* a, int left, int right, int pivot,Operation *op_qs)  //trebuie cu pointer la functii numarate operatiile
{
	int j = left - 1;
	if(op_qs != NULL)
		op_qs->count(3);
	swap(a[right], a[pivot]);
	for (int i = left; i <= right; i++) //in stanga vor fi elemente mai mici decat pivot,iar in dreapta mai mari decat pivot(nesortate)
	{
		if (op_qs != NULL)
			op_qs->count();
		if (a[i] <= a[right])
		{
			j++;
			if (op_qs != NULL)
				op_qs->count(3);
			swap(a[i], a[j]);
		}
	}
	return j; //pozitia pivotului
}
//best case pivot pe mijloc :sir crescator
// best case pivot in dreapta --
//
void quicksort(int* a, int left,int right,int currentSize,Operation *op)
{
	if (right <= left)
		return;
	int k = partition(a, left, right, right,op);//sau (left+right)/2 pt pivot la mijloc 
	quicksort(a, left, k - 1,currentSize,op);
	quicksort(a, k + 1, right,currentSize,op);
}
void sink(int* a, int n, int index,Operation *heap_op) //bottom-up
{
	while (2 * index < n) //porneste de la primul nod care are copii
	{
		int ind_child = 2 * index;
		if (ind_child + 1 <= n && a[ind_child + 1] < a[ind_child])
		{
			if (heap_op != NULL)
				heap_op->count();
			ind_child++;
		}
		if (a[index] > a[ind_child])
		{
			if (heap_op != NULL)
				heap_op->count(4);
			swap(a[index], a[ind_child]);
			index = ind_child;
		}
		else
			break;
	}
}
int* heapsort(int* a, int n,Operation *heap_op)
{
	int* b = new int[n];//alocare dinamica
	int index = 1;
	while (n > 1) //posibil >= 1 
	{
		if(heap_op != NULL)
			heap_op->count(4); //3 din swap si 1 de la b[index] = a[1]
		b[index] = a[1];
		index++;
		swap(a[1], a[n]);
		n--;
		sink(a, n, 1,heap_op);
	}
	if (heap_op != NULL)
		heap_op->count(1);
	b[index] = a[1];
	return b;
}
void insertionSort(int* arr, int left, int right, Operation* op)
{
	for (int i = left + 1; i <= right; i++)
	{
		if(op!= NULL)
			op->count();
		int aux = arr[i];
		int j = i - 1;
		while (arr[j] > aux && j >= left)
		{
			if (op != NULL)
				op->count(2);
			arr[j + 1] = arr[j];
			j--;
		}
		if (op != NULL)
			op->count();
		arr[j + 1] = aux;;
	}
}

void hibridQuickSort(int* arr, int left, int right,int prag, Operation* op1)
{
	if (left >= right)
		return;
	while (left < right)
	{
		if (right - left < prag) //pragul pt a face insertion sort
		{
			insertionSort(arr, left, right, op1);
			break;
		}
		else
		{
			int k = partition(arr, left, right, right, op1);
			if (k - left < right - k)
			{
				hibridQuickSort(arr, left, k - 1,prag, op1);
				left = k + 1;
			}
			else
			{
				hibridQuickSort(arr, k + 1, right,prag, op1);
				right = k - 1;
			}	
		}
	}
}
int randPartition(int* a, int left, int right) //returneaza pozitia random a pivotului
{
	int pivot = left + rand() % (right - left + 1);
	return partition(a, left, right, pivot, NULL);
}

int QuickSelect(int* a, int left, int right, int k)
{
	while (left <= right)
	{
		int pivot = randPartition(a, left, right);
		if (pivot == k) //k fiind pozitia elementului cerut din sirul dat
			return a[pivot];
		else if (pivot < k)
		{
			left = pivot + 1;
		}
		else
		{
			right = pivot - 1;
		}
	}
	return -1; //daca k nu este pozitie valida in sir
}
void demo_quickselect()
{
	int a[] = { 1,3,2,7,5 };
	int n = sizeof(a) / sizeof(int);
	int k = 3; //indexare de la 0 la n-1
	int number = QuickSelect(a, 0, n - 1, k);
	cout << "Numarul " << number << " se afla pe pozitia " << k << " in sirul dat.\n";
}
void demo_quicksort() {
	int a[] = { 1,3,2,7,5 };
	int n = sizeof(a) / sizeof(int);
	quicksort(a, 0, n-1,n,NULL);
	cout<<"Sir sortat cu QuickSort: ";
	for (int i = 0; i < n; i++)
	{
		cout << a[i] << " ";
	}
	cout << "\n";
}
void demo_heapsort()
{
	int x[] = { 0,1,3,2,7,5 }; //min heap -> parintele mai mic sau egal cu copii lui
	int size3 = 5;
	Operation op = profiler.createOperation("Demo_Heapsort",size3);
	cout<<"Sir sortat cu HeapSort: ";
	int* arr = heapsort(x, size3,&op);
	for (int i = 1; i <= size3; i++)
	{
		cout << arr[i] << " ";
	}
	cout << "\n";
}
void demo_hibridQuickSort()
{
	int prag = 10;
	int arr[] = { -4,-100,88,66,43,101,99,-57,102,0,-1,15 };
	int size = sizeof(arr) / sizeof(int);
	hibridQuickSort(arr, 0, size - 1,prag, NULL);
	cout << "Sir sortat cu QuickSort hibridizat: ";
	cout << "\n";
	for (int i = 0; i < size; i++)
	{
		cout << arr[i] <<" ";
	}
	cout << "\n";
}
void perf()
{
	int array[MAX_SIZE];
	int copy[MAX_SIZE];
	for (int i = STEP_SIZE; i < MAX_SIZE; i += 2*STEP_SIZE)
	{
		Operation op = profiler.createOperation("AverageHeapsortTotal", i);
		Operation op2 = profiler.createOperation("AverageQuicksortTotal", i);
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(array, i, -5000, 5000, false, 0);
			CopyArray(copy, array, MAX_SIZE);
			quicksort(array, 0, i-1,i,&op2);
			heapsort(copy, i, &op);
		}
	}
	profiler.divideValues("AverageHeapsortTotal", NR_TESTS);
	profiler.divideValues("AverageQuicksortTotal", NR_TESTS);
	profiler.createGroup("HeapSortVSQuickSort", "AverageHeapsortTotal", "AverageQuicksortTotal");
	profiler.reset();
	for (int i = STEP_SIZE; i < MAX_SIZE; i += STEP_SIZE)
	{
		Operation op2 = profiler.createOperation("BestQuicksortTotal", i);
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(array, i, -5000, 5000, false, 0);
			quicksort(array, 0, i - 1, i,&op2);
		}
	}
	profiler.divideValues("BestQuicksortTotal", NR_TESTS);
	profiler.reset();
	for (int i = STEP_SIZE; i < MAX_SIZE; i += STEP_SIZE)
	{
		Operation op2 = profiler.createOperation("WorstQuicksortTotal", i);
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(array, i, -5000, 5000, false, 2);
			quicksort(array, 0, i - 1, i,&op2);
		}
	}
	profiler.divideValues("WorstQuicksortTotal", NR_TESTS);
	profiler.reset();

	
	for (int i = STEP_SIZE; i < MAX_SIZE; i += STEP_SIZE)
	{
		for (int prag = 5; prag <= 50; prag += 5)
		{
			Operation prag_op = profiler.createOperation("PragOptim", prag);
			FillRandomArray(array, MAX_SIZE, -5000, 5000, 0);
			hibridQuickSort(array, 0, MAX_SIZE - 1, prag, &prag_op);
		}
	}
	profiler.divideValues("PragOptim", MAX_SIZE);
	profiler.reset();

	for (int i = STEP_SIZE; i < MAX_SIZE; i += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			//Operation qs1 = profiler.createOperation("QuickSort", i);
			Operation op2 = profiler.createOperation("Quicksort", i);
			Operation qs2 = profiler.createOperation("HibridQuickSort", i);
			FillRandomArray(array, i, -5000, 5000, false, 0);
			CopyArray(copy, array, i);
			quicksort(array, 0, i - 1, i,&op2);
			hibridQuickSort(copy, 0, i - 1, 30, &qs2);
		}
	}
	profiler.divideValues("HibridQuickSort", NR_TESTS);
	profiler.divideValues("Quicksort", NR_TESTS);
	profiler.createGroup("QuicksortVSHibridQuicksort", "Quicksort", "HibridQuickSort");
	profiler.reset();

	int array2[10000], copy2[10000];
	for (int i = STEP_SIZE; i < MAX_SIZE; i += STEP_SIZE)
	{
		
			FillRandomArray(array2, i, -5000, 5000, false, 0);
			CopyArray(copy2, array2, i);

			profiler.startTimer("TimerQuickSort", i);
			for (int k = 0; k < 10; k++)  //daca pun valori mai mari nu se mai termina programu,am stat si peste 10 min si tot nimic :))
			{
				quicksort(array2, 0, i - 1, i, NULL);
			}
			profiler.stopTimer("TimerQuickSort", i);

			profiler.startTimer("TimerHibridQuicksort", i);
			for (int k = 0; k < 10; k++)
			{
				hibridQuickSort(copy2, 0, i - 1, 30, NULL);
			}
			profiler.stopTimer("TimerHibridQuicksort", i);
			
	}
	//profiler.divideValues("TimerQuickSort", NR_TESTS);
	//profiler.divideValues("TimerHibridQuicksort", NR_TESTS);
	profiler.createGroup("TimerTotal", "TimerQuickSort", "TimerHibridQuicksort");

	profiler.showReport();
}
//void perf_all()
//{
//	perf(UNSORTED);
//	profiler.reset("best");
//	perf(ASCENDING);
//	profiler.reset("worst");
//	perf(DESCENDING);
//	profiler.showReport();
//}
int main()
{
	//demo_quicksort(); //demonstrare corectitudine algoritm
	//demo_heapsort();
	//demo_hibridQuickSort();
	//demo_quickselect();
	perf();
	return 0;
}