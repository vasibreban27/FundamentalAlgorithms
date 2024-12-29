/*
* @author Breban Vasile,group 8
* Cerinte de implementat :
*	1. DFS (5p) 
*	2.Sortare topologică (1p) 
*	3.Tarjan (2p)
*	4.Analiza performanței pentru DFS (2p)
* 
*	--DFS(depth-first-search) e un algoritm de cautare/parcurgere a unui graf in adancime, merge pe cat mai departe posibil dintr-un nod dat
*	--se poate implementa atat recursiv cat si iterativ folosind un stack
*	--complexitate : O(v+e) pentru un graf reprezentat prin lista de adiacenta si O(v^2) pentru un graf reprezentat prin matrice de adiacenta
*		--v este numarul de varfuri(noduri) iar e este numarul de muchii
*	--Sortarea topologica este ordonarea varfurilor intr-un graf orientat, fara cicluri , complexitate O(v+e)
	--Algoritmul lui Tarjan este un alg ce are la baza DFS-ul si detecteaza componentele tari conexe ale unui graf(fiecare nod este accesibil din orice alt nod)
		--complexitate tot O(v+e)
*	
*/

#include <iostream>
#include <stack>
#include <vector>
#include <set>
#include "Profiler.h"

struct node {
	int key, adjSize, color;
	node* adj[4500];
};
enum { WHITE, GREY, BLACK };
Profiler profiler;

using namespace std;

vector<vector<int>> scc;
stack<node*> nodeStack;
stack<int> topologicalSort;
int indices[4500], lowLink[4500];
bool onStack[4500];
bool cycle = false;
int index = 0;

void dfs_visit(node* start, Operation* op)
{
	op->count();
	start->color = GREY;
	for (int i = 0; i < start->adjSize; i++) {
		op->count();
		if (start->adj[i]->color == WHITE) //daca == GRAY avem ciclu,deci nu putem face sortare topologica
		{
			cout << "Muchie din arbore: " << start->key << " -> " << start->adj[i]->key << endl;
			dfs_visit(start->adj[i], op);
		}
		else if (start->adj[i]->color == GREY) {
				cycle = true;
			}
		topologicalSort.push(start->key);
		start->color = BLACK; //pt sortare topologica,punem intr-un stack nodurile start 
		//topologicalSort.push(start->key);
	}
}

void dfs(node* list[], int v, Operation* op) {
	for (int i = 0; i < v; i++)
	{
		list[i]->color = WHITE; //initializare noduri ca fiind nevizitate
	}
	for (int i = 0; i < v; i++)
	{
		op->count();
		if (list[i]->color == WHITE)
		{
			cout << "Incepere dfs din nodul: " << list[i]->key << endl;
			dfs_visit(list[i], op);
		}
	}
}
	
void print_topologic_sort() {
	if (cycle == true)
	{
		cout << "Graful are ciclu, sortarea topologica nu e posibila." << endl;
	}
	else
	{
		cout << "Sortarea topologica este: ";
		while (!topologicalSort.empty()) //daca stack-ul nu e gol
		{
			cout << topologicalSort.top() << " ";
			topologicalSort.pop(); //afisam si dam pop la stiva
		}
		cout << endl;
	}
}

void strong_connect(node* start)
{
	int indexStart = start->key;
	indices[indexStart] = lowLink[indexStart] = index++; //indices - index de descoperire //lowLink - cel mai mic index de descoperire accesibil din acel nod
	nodeStack.push(start); //stiva pt noduri
	onStack[indexStart] = true; //stiva pentru a marca daca nodul e deja in stiva

	for (int i = 0; i < start->adjSize; i++)  //explorare vecini , toate nodurile adiacente unui nod
	{
		node* neighbor = start->adj[i];
		int indexCurrent = neighbor->key;
		if (indices[indexCurrent] == -1)  //daca avem vecin nevizitat
		{
			strong_connect(neighbor); //apelam recursiv pe el
			lowLink[indexStart] = min(lowLink[indexStart], lowLink[indexCurrent]); //actualizam lowLink
		}
		else if (onStack[indexCurrent]) //daca e deja pe stiva
		{
			lowLink[indexStart] = min(lowLink[indexStart], indices[indexCurrent]); 
		}
	}

	if (lowLink[indexStart] == indices[indexStart]) //nodul este root-ul unei componente conexe
	{
		vector<int> component; //vector pentru a tine nodurile unei comp conexe
		node* w;
		do {
			 w = nodeStack.top();
			nodeStack.pop();
			onStack[w->key] = false;
			component.push_back(w->key);
		} while (w != start);
		scc.push_back(component); //stack de componente conexe
	}
}

void tarjan(node* graph[], int v) {
	for (int i = 0; i < v; i++)  //initializare array-uri
	{
		indices[i] = -1;
		lowLink[i] = -1;
		onStack[i] = false;
	}
	for (int i = 0; i < v; i++) {
		if (indices[graph[i]->key] == -1)
			strong_connect(graph[i]);
	}
}

void print_SCC() {
	cout << "Componentele puternic conexe sunt: " << endl;
	for (const auto& component : scc) {
		for (int node : component) {
			cout << node << " ";
		}
		cout << endl;
	}
}

void generate_rand_graph(node* graph[], int v, int e)
{
	set<pair<int, int>> edges; //pentru a avea muchii unice
	for (int i = 0; i < v; i++) //initializare graf
	{
		graph[i] = new node;
		graph[i]->key = i;
		graph[i]->color = WHITE;
		graph[i]->adjSize = 0;
	}
	// genereaza muchii astfel incat fiecare nod sa aiba cel putin o muchie
	for (int i = 0; i < v; i++) {
		int j = (i + 1) % v;
		graph[i]->adj[graph[i]->adjSize++] = graph[j];
		edges.insert({ i, j }); //adaug muchia i->j
	}
	while (edges.size() < e)  //generez random restul muchiilor pana la e
	{
		int v1 = rand() % v;
		int v2 = rand() % v;
		if (v1 != v2 && edges.find({ v1, v2 }) == edges.end()) //daca v1!=v2 si muchia nu a fost inserata deja
		{
			graph[v1]->adj[graph[v1]->adjSize++] = graph[v2]; //adaugare muchie in graf
			edges.insert({ v1, v2 });
		}
	}
}

void demo_dfs_topologicalSort() {
	const int n = 5;
	node* graph[n];
	for (int i = 0; i < n; i++) //initializare graf
	{
		graph[i] = new node;
		graph[i]->key = i;
		graph[i]->color = WHITE;
		graph[i]->adjSize = 0;
	}
	//creare muchii in graf
	graph[0]->adj[graph[0]->adjSize++] = graph[1];
	graph[1]->adj[graph[1]->adjSize++] = graph[2];
	graph[1]->adj[graph[1]->adjSize++] = graph[3];
	graph[2]->adj[graph[2]->adjSize++] = graph[4];
	graph[3]->adj[graph[3]->adjSize++] = graph[4];
	graph[3]->adj[graph[3]->adjSize++] = graph[1];

	cout << "Graful reprezentat prin lista de adiacenta: " << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "Nodul " << graph[i]->key << ": ";
		for (int j = 0; j < graph[i]->adjSize; j++)
		{
			cout << graph[i]->adj[j]->key << " ";
		}
		cout << endl;
	}
	Operation op1 = profiler.createOperation("dfs_count", n);
	cout << "Parcurgerea DFS a grafului:" << endl;
	dfs(graph, n, &op1);
	print_topologic_sort();
	for (int i = 0; i < n; i++)
	{
		delete graph[i];
	}
}

void demo_tarjan() {
	const int n = 5;
	node* graph[n];

	// Creare noduri
	for (int i = 0; i < n; i++) {
		graph[i] = new node;
		graph[i]->key = i;
		graph[i]->color = WHITE;
		graph[i]->adjSize = 0;
	}

	// Creare muchii
	graph[0]->adj[graph[0]->adjSize++] = graph[1];
	graph[1]->adj[graph[1]->adjSize++] = graph[2];
	graph[2]->adj[graph[2]->adjSize++] = graph[0]; // Ciclul 0 -> 1 -> 2 -> 0
	graph[3]->adj[graph[3]->adjSize++] = graph[4];
	graph[4]->adj[graph[4]->adjSize++] = graph[3]; // Ciclul 3 -> 4 -> 3

	// Afisam graful
	cout << "Graful reprezentat prin lista de adiacenta: " << endl;
	for (int i = 0; i < n; i++) {
		cout << "Nodul " << graph[i]->key << ": ";
		for (int j = 0; j < graph[i]->adjSize; j++) {
			cout << graph[i]->adj[j]->key << " ";
		}
		cout << endl;
	}
	tarjan(graph, n);
	print_SCC();
	for (int i = 0; i < n; i++) {
		delete graph[i];
	}
}

void demo_analysis() {
	int n;
	//variam numarul de muchii
	for (n = 1000; n <= 4500; n += 100) {
		Operation op5 = profiler.createOperation("dfs-edges", n);
		const int nr_vf = 100;
		node* graph[nr_vf];
		generate_rand_graph(graph, nr_vf, n);
		dfs(graph, nr_vf, &op5);
	}
	//variam nr de varfuri
	for (n = 100; n <= 200; n += 10) {
		Operation op6 = profiler.createOperation("dfs-vertices", n);
		node* graph[200];
		generate_rand_graph(graph, n, 4500);
		dfs(graph, n, &op6);
	}
	profiler.showReport();
}
int main() {
	demo_dfs_topologicalSort();
	//demo_tarjan();
	//demo_analysis();
	return 0;
}