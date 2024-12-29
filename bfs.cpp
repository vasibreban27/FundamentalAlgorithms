/*
* @author Breban Vasile , group 8
* BFS - cautare in latime
* 
* Cerinte:
*   1.Determinarea vecinilor unei celule
*   2.Implementarea algoritmului BFS
*   3.Afisarea arborelui BFS
*   4.Evaluarea performantei algoritmului BFS
*   5.Bonus: Determinarea celui mai scurt drum(shortest path)
* Explicarea codului am realizat-o prin adaugarea de comentarii pentru fiecare functie rezolvata.
* 
* Complexitate: - BFS - O(V+E) in general, iar pentru grafuri dense(sau complete unde nr muchii e = v^2) complexitatea va fi O(V^2)
*/
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <string.h>
#include "bfs.h"

using namespace std;
Profiler profiler;
int get_neighbors(const Grid* grid, Point p, Point neighb[])
{
    //dx = -1 vecin deasupra, dx=1 vecin de jos, dy=-1 vecin stanga, dy=1 vecin dreapta
    int dx[] = { -1,1,0,0 }; //pt randuri,axa Ox
    int dy[] = { 0,0,-1,1 }; //pt coloane,axa Oy
    int count = 0; //contor pt nr de vecini valizi
    for (int i = 0; i < 4; i++)
    {
        int newRow = p.row + dx[i]; //noua poz pe rand
        int newCol = p.col + dy[i]; //noua poz pe coloana
        if (newRow > 0 && newRow < grid->rows && newCol > 0 && newCol < grid->cols) //verif daca noua poz se afla in grila
        {
            if (grid->mat[newRow][newCol] != 1) //daca nu e perete,salvam coordonatele vecinului valid in neighb
            {
                neighb[count].row = newRow;
                neighb[count].col = newCol;
                count++;
            }
        }
    }
    return count;
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
}

void grid_to_graph(const Grid* grid, Graph* graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node* nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (grid->mat[i][j] == 0) {
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }
            else {
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (nodes[i][j] != NULL) {
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for (i = 0; i < graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if (graph->v[i]->adjSize != 0) {
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for (j = 0; j < graph->v[i]->adjSize; ++j) {
                if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0) {
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if (k < graph->v[i]->adjSize) {
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph* graph)
{
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph* graph, Node* s, Operation* op)
{
    for (int i = 0; i < graph->nrNodes; i++) //initializare graf
    {
        if (op != NULL)
            op->count(3);
        graph->v[i]->color = COLOR_WHITE; //nod nevizitat
        graph->v[i]->dist = INT_MAX;
        graph->v[i]->parent = NULL;
    }
    if (op != NULL)
        op->count(4);
    s->color = COLOR_GRAY; //initializare nod start //nod in parcurgere de vizitare
    s->dist = 0;
    s->parent = NULL;
    queue<Node*> que; //coada pt a tine toti vecinii unui nod
    que.push(s);
    while (!que.empty())  //parcurgere bfs
    {
        if (op != NULL)
            op->count(2);
        Node* current = que.front();
        que.pop();
        for (int i = 0; i < current->adjSize; i++) //cautam toti vecinii nevizitati ai nodului curent
        {
            if (op != NULL)
                op->count(1);
            Node* neighbor = current->adj[i];
            if (neighbor->color == COLOR_WHITE) //daca nu a fost vizitat
            {
                if (op != NULL)
                    op->count(4);
                neighbor->color = COLOR_GRAY;
                neighbor->dist = current->dist + 1;  //dist fata de nodul sursa
                neighbor->parent = current; //setam parintele pt arborele bfs
                que.push(neighbor);
            }
        }
        if (op != NULL)
            op->count(1);
        current->color = COLOR_BLACK; //nod complet vizitat explorat

    }
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();
}
void print_tree_recursive(int node, int* p, Point* repr, int n, int level) {

    if (repr == NULL)
        return;
    for (int i = 0; i < level; i++) {
        printf("       ");
    }
    //afisare nod curent
    printf("(%d, %d)\n", repr[node].row, repr[node].col);
    // gasire si afisare copii ai nodului curent
    for (int i = 0; i < n; i++) {
        if (p[i] == node) { //daca i este copil al nodului curent
            print_tree_recursive(i, p, repr, n, level + 1); 
        }
    }
}

void print_bfs_tree(Graph* graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int* p = NULL; //the parent array
    Point* repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int* transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        }
        else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            }
            else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                }
                else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        int start = -1;
        for (int i = 0; i < n; i++) {
            if (p[i] == -1) //gasesc radacina in vectorul de parinti
            {
                start = i;
            }
        }
        // TODO: pretty print the BFS tree
        print_tree_recursive(start, p, repr, n, 0);
    }

    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }

}

int shortest_path(Graph* graph, Node* start, Node* end, Node* path[])
{
    Operation op = profiler.createOperation("CountBFSShortestPath", 5000);
    bfs(graph, start, &op);
    if (end->dist == INT_MAX) //daca distanta e infinita, nu am o cale de la start->end
        return -1;
    int pathLength = 0; //contor nr de noduri in path
    Node* current = end;
    while (current != NULL)  //parcurgere inversa de la end->start pe legatura parinte
    {
        path[pathLength++] = current; //salvare nod in path
        current = current->parent;
    }

    for (int i = 0; i < pathLength / 2; i++) //parcurgere pentru a inversa drumul, sa il avem de la start->end
    {
        Node* temp = path[i];
        path[i] = path[pathLength - 1 - i];
        path[pathLength - 1 - i] = temp;
    }
    return pathLength;
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
}

int edge_exists(Node* u, Node* v)  //functie ce verifica existenta unei muchii in matricea de adiacenta
{
    for (int i = 0; i < u->adjSize; i++) {
        if (u->adj[i] == v) {
            return 1; //exista muchia
        }
    }
    return 0;
}

void add_random_edge(Graph* graph, int max_edges) {
    int u, v;
    do {
        u = rand() % graph->nrNodes;
        v = rand() % graph->nrNodes;
    } while (u == v || edge_exists(graph->v[u], graph->v[v]) || graph->v[u]->adjSize >= max_edges || graph->v[v]->adjSize >= max_edges);
    //generez pana cand u!=v sau muchia nu exista deja in graf 
    //nodul u nu are numarul maxim de muchii max_edges, la fel si v

    //adaugare muchie 
    graph->v[u]->adj[graph->v[u]->adjSize++] = graph->v[v];
    graph->v[v]->adj[graph->v[v]->adjSize++] = graph->v[u];
}

void generate_connected_graph(Graph* graph, int num_edges) {
    int nrNodes = graph->nrNodes;

    //pas 1: Creare spanning tree pentru a asigura conecectivitatea(conex)
    for (int i = 1; i < nrNodes; i++) {
        int parent = rand() % i; // conectam fiecare nod nou la un nod generat anterior
        graph->v[parent]->adj[graph->v[parent]->adjSize++] = graph->v[i];
        graph->v[i]->adj[graph->v[i]->adjSize++] = graph->v[parent];
    }

    // pas 2: adaugam random muchiile ramase
    int edges_added = nrNodes - 1; // in spanning tree avem deja nrNodes - 1 muchii
    while (edges_added < num_edges) {
        add_random_edge(graph, num_edges);
        edges_added++;
    }
}

void initialize_graph(Graph* graph, int nrNodes) {
    graph->nrNodes = nrNodes;
    graph->v = (Node**)malloc(nrNodes * sizeof(Node*));
    for (int i = 0; i < nrNodes; ++i) {
        graph->v[i] = (Node*)malloc(sizeof(Node));
        memset(graph->v[i], 0, sizeof(Node));
        // Allocate adjacency list for each node
        graph->v[i]->adj = (Node**)malloc(nrNodes * sizeof(Node*)); // Allocate maximum potential size
        graph->v[i]->adjSize = 0; // Initialize adjacency list size
    }
}

void performance() {
    int n, i;
    // variam numarul de muchii
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = profiler.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;  // nr fix de noduri
        initialize_graph(&graph, graph.nrNodes);

        generate_connected_graph(&graph, n);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // variam numarul de noduri
    for (n = 100; n <= 200; n += 10) {
        Operation op = profiler.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n; 
        initialize_graph(&graph, graph.nrNodes);

        // generam 4500 de muchii in acest graf
        generate_connected_graph(&graph, 4500);

        bfs(&graph, graph.v[0], &op);  
        free_graph(&graph);  
    }

    profiler.showReport();
}