#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct node {
    int value, weight;
    struct node *next;
} Node;

typedef struct graph {
    Node **vet; //List vector
    int vertex; //Number of vertex
    int edges;  //Number of edges
} *Graph;

Graph graph_boot(int num_vertex) {
    Graph G = malloc(sizeof (*G));
    G->vertex = num_vertex;
    G->edges = 0;

    G->vet = malloc(num_vertex * sizeof(Node*));
    for (int i = 0; i < num_vertex; i++) 
        G->vet[i] = NULL;
    
    return G;
}

void edge_insert(Graph G, int i, int j, int c) {
    Node *newI = malloc(sizeof(Node));
    newI->value = j;
    newI->weight = c;
    newI->next = G->vet[i];
    G->vet[i] = newI;
    G->edges++;
}

void print_graph(Graph G) {
    for (int i = 0; i < G->vertex; i++) {
        Node *aux = G->vet[i];
        
        printf("[%c]->",i+65);
        while (aux) {
            printf("(%c)->",aux->value+65);
            aux = aux->next;
        }
        printf("NULL\n");
    }
}


void release_graph(Graph G) {
    for (int i = 0; i < G->vertex; i++) {
        Node *aux = G->vet[i];
        while (aux) {
            Node *del = aux;
            aux = aux->next;
            free(del);
        }
        G->vet[i] = NULL;
    }
    free(G->vet);
    G->vet = NULL;
    free(G);
}

void BellmanFord_boot(Graph G, int *distance, int *parent, int source) {
    for (int i = 0; i < G->vertex; i++) {
        distance[i] = INT_MAX/2;
        parent[i] = -1;
    }
    
    distance[source] = 0;
}

void relax(Graph G, int *distance, int *parent, int u) {
    Node *aux = G->vet[u];

    while (aux) {
       if (distance[aux->value] > aux->weight + distance[u]) {
            distance[aux->value] = aux->weight + distance[u];
            parent[aux->value] = u;
       }
       aux = aux->next;
    }
}

void print_path(int parent[], int v) {
    if (v < 0)
        return;
    print_path(parent, parent[v]);
    printf("%d ", v);
}

void BellmanFord(Graph G, int source) {
    int *distance = malloc(G->vertex * sizeof(int));
    int *parent = malloc(G->vertex * sizeof(int));
    BellmanFord_boot(G, distance, parent, source);

    for (int i = 0; i < G->vertex-1; i++) {
            for (int j = 0; j < G->vertex; j++)
                relax(G, distance, parent, j);
    }

    for (int i = 0; i < G->vertex; i++) {
        Node *aux = G->vet[i];

        while (aux) {
            if (distance[aux->value] > aux->weight + distance[i]) {
                printf("The Graph have a negative cicle\n");
                return;
            }
            aux = aux->next;
        }
    }

    printf("Shortest paths from the city %d:\n", source);
    for (int i = 0; i < G->vertex; i++) {
        printf("Path to %d: ", i);
        print_path(parent, i);
        printf("\nDistance: %d\n", distance[i]);
    }   
    
}

int main() {
    int n, m, o, d, c;

    printf("\nNumber of cities: ");
    scanf("%d", &n);

    Graph G = graph_boot(n);

    printf("\nNumber of routes: ");
    scanf("%d", &m);

    printf("\nInsert the %d routes as follows: origin destination cost\n", m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &o, &d, &c);
        edge_insert(G, o, d, c);
    }

    printf("\nOrigin City: ");
    scanf("%d", &o);

    BellmanFord(G, o);
    
    release_graph(G);

    return 0;
}