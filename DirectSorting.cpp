/*
@author Breban Vasile
@group 30228

Problema rezolvata : Scrierea algoritmilor de sortare directa : selection sort,bubble sort si insertion sort.
Numararea asignarilor si compararilor celor 3 algoritmi si diferentele in cele 3 cazuri:worst,average si best.

*BONUS : binary insertion sort si compararea acestuia cu ceilalti 3 algoritmi
* Acest algoritm e mai eficient pentru sortarea sirului in cazul best si average fata de celelalte 3.
BEST CASE: sirul sortat crescator pentru cei 4 algoritmi
WORST CASE: siruri sortate descrescator pentru insertion,bubble si binary dar crescator cu cel mai mare element pe prima pozitie pentru selection sort
AVERAGE CASE: siruri aranjate random 

Selection sort este un algoritm instabil,celelalte fiind stabile
Complexitate algoritmi: O(n^2)
Diferenta pe cele 3 cazuri(intre cei 4 algoritmi)se poate observa in grafice
*/

#include <iostream>
#include <cmath>
#include "Profiler.h"
using namespace std;
Profiler profiler;
void my_swap(int* a, int* b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}
void selectionSort(int a[], int n)
{
	Operation comp_sel = profiler.createOperation("SelectionSortComp", n);
	Operation as_sel = profiler.createOperation("SelectionSortAssignments", n);
	for (int i = 0; i < n; i++)
	{
		int aux = i;
		for (int j = i + 1; j < n; j++)
		{
			comp_sel.count();
			if (a[aux] > a[j])
			{
				aux = j;
			}
		}
		if (aux != i)
		{
			as_sel.count(3);
			my_swap(&a[i], &a[aux]);
		}
	}

}
void bubbleSort(int arr[], int n)
{
	Operation comp_bub = profiler.createOperation("BubbleSortComp", n);
	Operation as_bub = profiler.createOperation("BubbleSortAssignments", n);
	int ok;
	do
	{
		ok = 1;
		for (int i = 0; i < n - 1; i++)
		{
			comp_bub.count();
			if (arr[i] > arr[i + 1])
			{
				as_bub.count(3);
				my_swap(&arr[i], &arr[i + 1]);
				ok = 0;
			}
		}
	} while (!ok);
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
		arr[j+1] = aux;
	}
}
void showArr(int a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
}

int binarySearch(int* arr, int elem, int low, int high)
{
	while (low <= high)
	{
		int mid = low + (high-low) / 2;
		if (arr[mid] == elem)
		{
			return mid + 1;
		}
		else if (elem > arr[mid])
			low = mid + 1;
		else
			high = mid - 1;
	}
	return low;
}
void binaryInsertionSort(int* a, int n)
{
	Operation as_binary = profiler.createOperation("BinarySortAssignments", n);
	Operation comp_binary = profiler.createOperation("BinarySortComp", n);
	int i, j, loc, k, selected;
	for (i = 1; i < n; i++)
	{
		j = i-1;
		as_binary.count();
		selected = a[i];
		comp_binary.count(2);
		loc = binarySearch(a, selected, 0, j); //gasim poz unde trebuie inserat elementul
		while (j >= loc)  //mutam elementele la dreapta pt a face loc
		{
			as_binary.count();
			a[j + 1] = a[j];
			j--;
 		}
		as_binary.count();
		a[j + 1] = selected;
	}
}

int main()
{
	int arr1[] = { 1,6,4,2,9,5 };
	int n = sizeof(arr1) / sizeof(int);
	insertionSort(arr1, n);
	printf("Sirul sortat cu insertion sort este: \n");
	showArr(arr1, n);

	int arr2[] = { 1,6,11,20,9,5 };
	int m = sizeof(arr2) / sizeof(int);
	bubbleSort(arr2, m);
	printf("Sirul sortat cu bubble sort este: \n");
	showArr(arr2, m);

	int arr3[] = { 1,16,4,25,9,5 };
	int p = sizeof(arr3) / sizeof(int);
	selectionSort(arr3, p);
	printf("Sirul sortat cu selection sort este: \n");
	showArr(arr3, p);

	int arr4[] = { 2,1,9,5,100,85 };
	int z = sizeof(arr4) / sizeof(int);
	binaryInsertionSort(arr4, z);
	printf("Sirul sortat cu binary insertion sort este: \n");
	showArr(arr4, z);

	int a[10000], b[10000], c[10000],c2[10000];
	for (int i = 100; i < 10000; i += 100)
	{
		FillRandomArray(a, i, -5000, 5000, false, 0);
		selectionSort(a, i);
		FillRandomArray(b, i, -5000, 5000, false, 0);
		bubbleSort(b, i);
		FillRandomArray(c, i, -5000, 5000, false, 0);
		insertionSort(c, i);
		FillRandomArray(c2, i, -5000, 5000, false, 0);
		binaryInsertionSort(c2, i);
	}
	profiler.addSeries("SelectionSort_AVERAGE_SERIES","SelectionSortComp","SelectionSortAssignments");
	profiler.addSeries("BubbleSort_AVERAGE_SERIES", "BubbleSortComp", "BubbleSortAssignments");
	profiler.addSeries("InsertionSort_AVERAGE_SERIES", "InsertionSortComp", "InsertionSortAssignments");
	profiler.addSeries("BinaryInsertionSort_AVERAGE_SERIES", "BinarySortComp", "BinarySortAssignments");
	
	profiler.createGroup("SelectionSort_AVERAGE", "SelectionSortComp", "SelectionSortAssignments");
	profiler.createGroup("BubbleSort_AVERAGE", "BubbleSortComp", "BubbleSortAssignments");
	profiler.createGroup("InsertionSort_AVERAGE", "InsertionSortComp", "InsertionSortAssignments");
	profiler.createGroup("BinaryInsertionSort_AVERAGE", "BinarySortComp", "BinarySortAssignments");
	profiler.createGroup("AVERAGE_OVERALL", "SelectionSort_AVERAGE_SERIES", "BubbleSort_AVERAGE_SERIES", "InsertionSort_AVERAGE_SERIES","BinaryInsertionSort_AVERAGE_SERIES");

	profiler.reset("SelectionSortComp");
	profiler.reset("SelectionSortAssignments");
	profiler.reset("BubbleSortComp");
	profiler.reset("BubbleSortAssignments");
	profiler.reset("InsertionSortComp");
	profiler.reset("InsertionSortAssignments");
	profiler.reset("BinarySortAssignments");
	profiler.reset("BinarySortComp");

	int d[10000], e[10000], f[10000],f2[10000];
	for (int i = 100; i < 10000; i += 100)
	{
		FillRandomArray(d, i, -5000, 5000, false, 1);
		selectionSort(d, i);
		FillRandomArray(e, i, -5000, 5000, false, 1);
		bubbleSort(e, i);
		FillRandomArray(f, i, -5000, 5000, false, 1);
		insertionSort(f, i);
		FillRandomArray(f2, i, -5000, 5000, false, 1);
		binaryInsertionSort(f2, i);
	}
	profiler.addSeries("SelectionSort_BEST_SERIES", "SelectionSortComp", "SelectionSortAssignments");
	profiler.addSeries("BubbleSort_BEST_SERIES", "BubbleSortComp", "BubbleSortAssignments");
	profiler.addSeries("InsertionSort_BEST_SERIES", "InsertionSortComp", "InsertionSortAssignments");
	profiler.addSeries("BinaryInsertionSort_BEST_SERIES", "BinarySortComp", "BinarySortAssignments");

	profiler.createGroup("SelectionSort_BEST", "SelectionSortComp", "SelectionSortAssignments");
	profiler.createGroup("BubbleSort_BEST", "BubbleSortComp", "BubbleSortAssignments");
	profiler.createGroup("InsertionSort_BEST", "InsertionSortComp", "InsertionSortAssignments");
	profiler.createGroup("BinaryInsertionSort_BEST", "BinarySortComp", "BinarySortAssignments");
	profiler.createGroup("BEST_OVERALL", "SelectionSort_BEST_SERIES", "BubbleSort_BEST_SERIES", "InsertionSort_BEST_SERIES","BinaryInsertionSort_BEST_SERIES");
	
	profiler.reset("SelectionSortComp");
	profiler.reset("SelectionSortAssignments");
	profiler.reset("BubbleSortComp");
	profiler.reset("BubbleSortAssignments");
	profiler.reset("InsertionSortComp");
	profiler.reset("InsertionSortAssignments");
	profiler.reset("BinarySortAssignments");
	profiler.reset("BinarySortComp");

	int g[10000], h[10000], k[10000],k2[10000];
	for (int i = 100; i < 10000; i += 100)
	{
		FillRandomArray(g, i, -5000, 5000, false, 1);
		g[0] = 5000;
		selectionSort(g, i);
		FillRandomArray(h, i, -5000, 5000, false, 2);
		bubbleSort(h, i);
		FillRandomArray(k, i, -5000, 5000, false, 2);
		insertionSort(k, i);
		FillRandomArray(k2, i, -5000, 5000, false, 2);
		binaryInsertionSort(k2, i);
	}
	profiler.addSeries("SelectionSort_WORST_SERIES", "SelectionSortComp", "SelectionSortAssignments");
	profiler.addSeries("BubbleSort_WORST_SERIES", "BubbleSortComp", "BubbleSortAssignments");
	profiler.addSeries("InsertionSort_WORST_SERIES", "InsertionSortComp", "InsertionSortAssignments");
	profiler.addSeries("BinaryInsertionSort_WORST_SERIES", "BinarySortComp", "BinarySortAssignments");

	profiler.createGroup("SelectionSort_WORST", "SelectionSortComp", "SelectionSortAssignments");
	profiler.createGroup("BubbleSort_WORST", "BubbleSortComp", "BubbleSortAssignments");
	profiler.createGroup("InsertionSort_WORST", "InsertionSortComp", "InsertionSortAssignments");
	profiler.createGroup("BinaryInsertionSort_WORST", "BinarySortComp", "BinarySortAssignments");

	profiler.createGroup("A_WORST_OVERALL", "SelectionSort_WORST_SERIES", "BubbleSort_WORST_SERIES", "InsertionSort_WORST_SERIES","BinaryInsertionSort_WORST_SERIES");

	profiler.showReport();
	return 0;
}