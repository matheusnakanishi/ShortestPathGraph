#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

//Graph Structs
typedef struct adjacencia {
    int vertice, peso;
    struct adjacencia *prox;
} Adj;

typedef struct {
    Adj *cab;
} Vertice;

typedef struct grafo {
    int vertices, arestas;
    Vertice *adj;
} Grafo;

//Graph Functions
Grafo* inicializar_grafo(int vertices) {
    Grafo *grafo = (Grafo*)malloc(sizeof(Grafo));

    grafo->vertices = vertices;
    grafo->arestas = 0;

    grafo->adj = (Vertice*)malloc(vertices * sizeof(Vertice));  //List vector alocation

    for (int i = 0; i < vertices; i++) 
        grafo->adj[i].cab = NULL;
    
    return grafo;
}

void inserir_aresta(Grafo *grafo, int origem, int destino, int peso) {
    Adj *aresta = (Adj*)malloc(sizeof(Adj));

    aresta->vertice = destino;
    aresta->peso = peso;
    aresta->prox = grafo->adj[origem].cab;

    grafo->adj[origem].cab = aresta;
    grafo->arestas++;
}

void imprimir_grafo(Grafo *grafo) {
    for (int i = 0; i < grafo->vertices; i++) {
        printf("\n[%d]: ", i);

        Adj *aux = grafo->adj[i].cab;
        while (aux) {
            printf("(%d|%d)->", aux->vertice, aux->peso);
            aux = aux->prox;
        }
    }
}

void liberar_grafo(Grafo *grafo) {
    for (int i = 0; i < grafo->vertices; i++) {
        Adj *aux = grafo->adj[i].cab;

        while (aux) {
            Adj *delete = aux;
            aux = aux->prox;
            free(delete);
        }
    }

    free(grafo->adj);
    free(grafo);
}

//Dijkstra Functions
bool existe_aberto(Grafo *grafo, bool *aberto) {
    for (int i = 0; i < grafo->vertices; i++) {
        if (aberto[i])
            return true;
    }
    return false;
}

void inicializar_Dijkstra(Grafo *grafo, int *distancia, int *pai, bool *aberto, int s) {
    for (int i = 0; i < grafo->vertices; i++) {
        distancia[i] = INT_MAX/2;
        pai[i] = -1;
        aberto[i] = true;
    }
    distancia[s] = 0;
}

int menor_distancia(Grafo *grafo, bool *aberto, int *distancia) {
    int i;

    for (i = 0; i < grafo->vertices; i++) {
        if (aberto[i])
            break;
    }

    if (i == grafo->vertices)
        return -1;

    int menor = i;
    
    for (i = menor + 1; i < grafo->vertices; i++) {
        if (aberto[i] && (distancia[menor] > distancia[i]))
            menor = i;
    }
    
    return menor;
}

void relaxa(Grafo *grafo, int *distancia, int *pai, int u, int v) {
    Adj *adj = grafo->adj[u].cab;

    while (adj && adj->vertice != v)
        adj = adj->prox;

    if (adj) {
        if (distancia[v] > distancia[u] + adj->peso) {
            distancia[v] = distancia[u] + adj->peso;
            pai[v] = u;
        }
    }
}

void imprimir_distancia(int v, int *distancia) {
    for (int i = 0; i < v; i++) 
        printf("%d: %d\n", i, distancia[i]);
}

void Dijkstra(Grafo *grafo, int s) {
    int *distancia = (int*)malloc(grafo->vertices * sizeof(int));
    int *pai = (int*)malloc(grafo->vertices * sizeof(int));
    bool *aberto = (bool*)malloc(grafo->vertices * sizeof(bool));

    inicializar_Dijkstra(grafo, distancia, pai, aberto, s);

    while (existe_aberto(grafo, aberto)) {
        int u = menor_distancia(grafo, aberto, distancia);
        aberto[u] = false;
        Adj *adj = grafo->adj[u].cab;

        while (adj) {
            relaxa(grafo, distancia, pai, u, adj->vertice);
            adj = adj->prox;
        }  
    }
    
    imprimir_distancia(grafo->vertices, distancia);
}


int main() {
    Grafo *grafo = inicializar_grafo(5);

    inserir_aresta(grafo, 0, 1, 2);
    inserir_aresta(grafo, 1, 2, 3);
    inserir_aresta(grafo, 2, 3, 5);
    inserir_aresta(grafo, 3, 4, 3);
    inserir_aresta(grafo, 1, 4, 4);
    inserir_aresta(grafo, 0, 4, 3);

    inserir_aresta(grafo, 1, 0, 2);
    inserir_aresta(grafo, 2, 1, 3);
    inserir_aresta(grafo, 3, 2, 5);
    inserir_aresta(grafo, 4, 3, 3);
    inserir_aresta(grafo, 4, 1, 4);
    inserir_aresta(grafo, 4, 0, 3);

    inserir_aresta(grafo, 1, 4, 6);
    inserir_aresta(grafo, 4, 1, 6);

    Dijkstra(grafo, 0);
    //imprimir_grafo(grafo);

    liberar_grafo(grafo);

    return 0;
}
