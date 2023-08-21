#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definir as estruturas para o grafo para o algoritmo de Prim usando vetor de adjacências

// Estrutura para representar uma aresta

typedef struct Edge
{
    int src, dest, weight;
} Edge;

// Estrutura para representar um grafo

typedef struct Graph
{
    // V-> Número de vértices, E-> Número de arestas
    int V, E;

    // O grafo é representado como um vetor de arestas
    Edge *edge;
} Graph;

// Cria um grafo com V vértices e E arestas

Graph *createGraph(int V, long E)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;

    graph->edge = (Edge *)malloc(graph->E * sizeof(Edge));

    return graph;
}

// Estrutura para representar um subconjunto para o algoritmo de Prim

typedef struct subset
{
    int parent;
    int rank;
} subset;

// Função para encontrar o conjunto de um elemento i

int find(subset subsets[], int i)
{
    // Encontrar o conjunto e fazer o elemento i apontar para o conjunto
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// Função que faz a união de dois conjuntos de x e y

void Union(subset subsets[], int x, int y)
{
    int xroot = find(subsets, x); // Encontrar o conjunto de x
    int yroot = find(subsets, y); // Encontrar o conjunto de y

    // Anexar a árvore de menor rank sob a raiz da árvore de maior rank
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // Se os ranks forem iguais, então faça um deles como raiz e incremente seu rank por 1
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Função que compara duas arestas de acordo com seus pesos

int compare(const void *a, const void *b)
{
    Edge *a1 = (Edge *)a;
    Edge *b1 = (Edge *)b;
    return a1->weight > b1->weight;
}

// Função principal do algoritmo de Prim

void primMST(Graph *graph, FILE *saida)
{
    int V = graph->V; // Número de vértices no grafo
    Edge result[V];   // Guarda o resultado da MST
    int e = 0;        // Variável auxiliar para o resultado
    int i = 0;        // Variável auxiliar para o grafo

    // Ordenar todas as arestas em ordem não decrescente de acordo com seus pesos
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), compare);

    // Alocar memória para criar V subconjuntos
    subset *subsets = (subset *)malloc(V * sizeof(subset));

    // Criar V subconjuntos com elementos individuais
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Número de arestas a serem tomadas é igual a V-1
    while (e < V - 1)
    {
        // Pegar a menor aresta e incrementar o index para a próxima iteração
        Edge next_edge = graph->edge[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        // Se incluir essa aresta não causa ciclo, inclua-a no resultado e incremente o index de resultado para o próximo vértice
        if (x != y)
        {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
        // Senão, descarte a aresta
    }

    // Escrever o conteúdo do resultado para um arquivo de saída
    for (i = 0; i < e; ++i)
    {
        fprintf(saida, "%d %d %d\n", result[i].src, result[i].dest, result[i].weight);
    }

    return;
}

// Função principal lendo o arquivo de entrada

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(1);
    }

    FILE *entrada = fopen(argv[1], "r");
    int V;
    int E;
    fscanf(entrada, "%d %d", &V, &E);
    if (V > 5000)
    {
        printf("Number of vertices must be less than 5000\n");
        exit(1);
    }

    Graph *graph = createGraph(V, E);
    /*     for (int i = 0; i < E; i++)
        {
            fscanf(entrada, "%d %d %d", &graph->edge[i].src, &graph->edge[i].dest, &graph->edge[i].weight);
        } */

    // rewrite the above for loop using pointer arithmetic

    for (int i = 0; i < E; i++)
    {
        fscanf(entrada, "%d %d %d", &((graph->edge + i)->src), &((graph->edge + i)->dest), &((graph->edge + i)->weight));
    }

    FILE *saida = fopen(argv[2], "w");

    primMST(graph, saida);
    fclose(entrada);
    fclose(saida);
    return 0;
}
