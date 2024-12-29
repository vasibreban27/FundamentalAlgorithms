/*
* @author Breban Vasile , group 8(30228)
*	Multimi disjuncte
* Cerinte:
*	1.Implementare corectă a MAKE_SET, UNION și FIND_SET (5p) 
*	2.Implementarea corectă și eficientă a algoritmului lui Kruskal 
*	3.Evaluarea operațiilor pe mulțimi disjuncte (MAKE, UNION, FIND) folosind 
*		algoritmului lui Kruskal
*
*	Pasi de implementare:
*		-implementarea celor 3 functii : make-set(creează o mulțime nouă ce conține elementul x) , union(realizează reuniunea dintre mulțimea care îl conține pe x 
*		și mulțimea care îl conține pe y) si find_set(caută mulțimea în care se afla x)
*		-demonstrarea celor 3 functii(in demo_func)
*		-implementarea algoritmului lui kruskal -- algoritmul lui Kruskal găsește o pădure de întindere minimă a unui grafic nedirecționat ponderat cu muchii. 
*			Dacă graficul este conectat, acesta găsește un arbore de acoperire minim. Este un algoritm de tip greedy care în fiecare pas adaugă pădurii 
*			marginea cea mai mică, care nu va forma un ciclu.
*		-evaluarea operatiilor - pentru fiecare n, am creat un graf conex neorientat cu 4*n muchii ( prima data am generat un spanning tree cu n muchii pt a a sigura conexitatea
*			iar mai apoi am generat restul muchiilor aleator, am folosit operatia set<pair> ce asigura unicitatea muchiilor)
* Complexitate : 
*	-cele 3 functii : find_set,make_set si union au complexitate constanta O(1)
*	-algoritmul lui kruskal - O(eloge) - e fiind numarul de muchii - sortarea muchiilor dupa greutate este cea mai costisitoare operatie,
*		iar parcurgerea muchiilor este O(e)	
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include "Profiler.h"
Profiler profiler;
struct node {
	int rank, key; //rank este inaltimea numarata in noduri
};
node parent[10005];

struct edge {
	int u, v, weight;
};

struct Graph {
	int v, e;
	edge* edges;
};//nu am folosit aceasta structura pana la urma

using namespace std;

void make_set(int x, Operation* op1)
{
	op1->count(2);
	parent[x].key = x;
	parent[x].rank = 1;
}

int find_set(int x, Operation* op2) //complexitate constanta
{
	op2->count();
	if (x == parent[x].key) //conditie de oprire
		return x;
	op2->count();
	parent[x].key = find_set(parent[x].key, op2);
	return parent[x].key;

}

void UNION(int x, int y, Operation* op3) {
	
	int rootX = find_set(x, op3);
	int rootY = find_set(y, op3);
	op3->count();
	if (rootX != rootY) {
		// Union dupa rank: atasam arborele mai mic la cel mai mare
		op3->count();
		if (parent[rootX].rank < parent[rootY].rank) {
			op3->count();
			parent[rootX].key = rootY;  //  rootY parintele lui rootX
		}
		else {
			op3->count();
			if (parent[rootX].rank > parent[rootY].rank) {
				op3->count();
				parent[rootY].key = rootX;  // rootX parintele lui rootY.
			}
			else {
				op3->count(2);
				// daca rankurile sunt egale, facem o radacina parintele celeilalte si crestem rankul noii radacini
				parent[rootY].key = rootX;
				parent[rootX].rank++;
			}
		}

	}
}

void display_sets(int n, Operation* op4)
{

	bool displayed[10005] = { false };
	for (int i = 1; i <= n; ++i) {
		int root = find_set(i, op4); // gaseste reprezentantul multimii
		if (!displayed[root]) { // daca reprezentantul nu a fost deja afisat
			cout << "Set reprezentat de " << root << ": ";
			for (int j = 1; j <= n; ++j) {
				if (find_set(j, op4) == root) { // afisam toate elementele cu acelasi reprezentant
					cout << j << " ";
				}
			}
			cout << endl;
			displayed[root] = true; // marcam reprezentantul ca afisat
		}
	}
}

bool compare(const edge& a, const edge& b)
{
	return a.weight < b.weight;
}

void kruskal(int v, int e, edge* edges, Operation* op1, Operation* op2, Operation* op3)
{
	for (int i = 1; i <= v; i++) {
		make_set(i, op1);
	}

	// Sortare muchii dupa weight
	sort(edges, edges + e, compare);

	cout << "Sortarea muchiilor dupa greutate:" << endl;
	for (int i = 0; i < e; i++) {
		cout << edges[i].u << "--" << edges[i].v << " (Weight: " << edges[i].weight << ")" << endl;
	}

	vector<edge> mst;  //vector pt a salva muchiile necesare pt MST
	int mst_cost = 0;

	cout << "Procesarea muchiilor pentru MST:" << endl;
	for (int i = 0; i < e; i++) {
		int u1 = edges[i].u;
		int v1 = edges[i].v;
		int weight = edges[i].weight;

		int rootU = find_set(u1, op2);
		int rootV = find_set(v1, op2);

		// daca u si v sunt in submultimi diferite, adaugam muchia la MST
		if (rootU != rootV) {
			mst.push_back(edges[i]);
			UNION(u1, v1, op3);
			mst_cost += weight;
			cout << "Muchia aleasa: " << u1 << "--" << v1 << " (Weight = " << weight << ")" << endl;

			if (mst.size() == v - 1) {
				break;  // oprire cand avem v-1 muchii in MST
			}
		}
	}
	// afisare finala MST
	cout << "Muchii in MST:" << endl;
	for (auto e : mst) {
		cout << e.u << "--" << e.v << " (Weight:" << e.weight << ")" << endl;
	}
	cout << "Costul total al MST: " << mst_cost << endl;
}

void demo_func()
{
	Operation op = profiler.createOperation("Demo", 10);
	for (int i = 1; i <= 10; ++i) {
		make_set(i, &op);
	}
	cout << "Multimile initiale:" << endl;
	display_sets(10, &op);

	UNION(1, 2, &op);
	UNION(2, 3, &op);
	UNION(4, 5, &op);
	UNION(6, 7, &op);
	UNION(3, 7, &op);

	cout << "\nDupa operatiile UNION:" << endl;
	display_sets(10, &op);

	// Test FIND_SET pentru 6 elemente
	cout << "\nReprezentanti pentru cateva elemente:" << endl;
	cout << "Reprezentant pentru 1: " << find_set(1, &op) << endl;
	cout << "Reprezentant pentru 2: " << find_set(2, &op) << endl;
	cout << "Reprezentant pentru 3: " << find_set(3, &op) << endl;
	cout << "Reprezentant pentru 6: " << find_set(6, &op) << endl;
	cout << "Reprezentant pentru 7: " << find_set(7, &op) << endl;
	cout << "Reprezentant pentru 5: " << find_set(5, &op) << endl;
}

void demo_kruskal()
{
	Operation op = profiler.createOperation("KruskalDemo", 10);
	int v = 5;
	int e = 9;

	// Lista de muchii (u, v, weight)
	edge edges[] = {
		{1, 2, 4}, {1, 3, 1}, {1, 4, 7},
		{2, 3, 2}, {2, 5, 5}, {3, 4, 3},
		{3, 5, 8}, {4, 5, 6}, {2, 4, 9}
	};

	cout << "Muchiile grafului:" << endl;
	for (auto e : edges) {
		cout << e.u << " -- " << e.v << " (Weight: " << e.weight << ")" << endl;
	}
	cout << endl;

	kruskal(v, e, edges, &op, &op, &op);
}

void demo_analysis()
{
	for (int n = 100; n <= 10000; n += 100)
	{
		for (int test = 0; test < 5; test++)
		{
			Operation op1 = profiler.createOperation("MakeSetOp", n);
			Operation op2 = profiler.createOperation("FindSetOp", n);
			Operation op3 = profiler.createOperation("UnionOp", n);

			int nr = n * 4; // nr total de muchii
			edge* edges = new edge[nr];
			vector<edge> edgeList;
			set<pair<int, int>> uniqueEdges; //pereche pt a seta muchiile si a evita duplicatele

			// pas 1: cream un spanning tree pt a asigura conectivitatea
			for (int i = 1; i < n; ++i)
			{
				int u = i;
				int v = rand() % i; // conectam fiecare nod cu unul generat anterior
				int weight = rand() % nr;
				edges[i - 1] = { u, v, weight }; // adaugam muchie la spanning tree
				uniqueEdges.insert({ min(u, v), max(u, v) });  //inserez muchia o singura data, cu u<v (muchia (1,2) este si (2,1) dar o inseram o singura data)
			}

			// pas 2: adaugam muchii random pt a ajunge la 'nr' muchii
			for (int i = n - 1; i < nr; ++i)
			{
				int u, v;
				do
				{
					u = rand() % n;
					v = rand() % n;
				} while (u == v || uniqueEdges.count({ min(u, v), max(u, v) })); // verific daca u=v sau daca muchia deja exista
				int weight = rand() % nr;
				edges[i] = { u, v, weight };
				uniqueEdges.insert({ min(u, v), max(u, v) });
			}
			kruskal(n, nr, edges, &op1, &op2, &op3);
			delete[] edges;
		}
	}
	profiler.divideValues("MakeSetOp", 5);
	profiler.divideValues("FindSetOp", 5);
	profiler.divideValues("UnionOp", 5);
	profiler.createGroup("MultimiDisjuncteOp", "MakeSetOp", "FindSetOp", "UnionOp");
	profiler.showReport();
}

int main()
{
	//demo_func();
	demo_kruskal();
	//demo_analysis();
	return 0;
}