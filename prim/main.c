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
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Função que faz a união de dois conjuntos de x e y
void Union(subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (xroot != yroot)
    {
        if (subsets[xroot].rank < subsets[yroot].rank)
            subsets[xroot].parent = yroot;
        else if (subsets[xroot].rank > subsets[yroot].rank)
            subsets[yroot].parent = xroot;
        else
        {
            subsets[yroot].parent = xroot;
            subsets[xroot].rank++;
        }
    }
}

// Função que compara duas arestas de acordo com seus pesos

int compare(const void *a, const void *b)
{
    Edge *a1 = (Edge *)a;
    Edge *b1 = (Edge *)b;
    return a1->weight - b1->weight;
}

// Função principal do algoritmo de Prim; Leve em conta que a escrita deve conter o valor do peso sem altera-lo

void primMST(Graph *graph, FILE *saida)
{
    int V = graph->V;
    Edge result[V]; // Array para armazenar a MST
    int e = 0;      // Index usado para o resultado
    int i = 0;      // Index usado para as arestas ordenadas

    // Ordena todas as arestas em ordem crescente de acordo com seus pesos
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), compare);

    // Aloca memória para criar V subconjuntos
    subset *subsets = (subset *)malloc(V * sizeof(subset));

    // Cria V subconjuntos com elementos individuais
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Número de arestas a serem tomadas é igual a V-1
    while (e < V - 1 && i < graph->E)
    {
        // Pega a próxima aresta de acordo com a ordenação
        Edge next_edge = graph->edge[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        // Se incluir essa aresta não forma um ciclo, inclua-a na MST e incremente o index do resultado para a próxima aresta
        if (x != y)
        {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
    }

    // Escreve a MST no arquivo de saída
    for (i = 0; i < e; ++i)
    {
        fprintf(saida, "%d %d\n", result[i].src, result[i].dest);
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

    for (int i = 0; i < E; i++)
    {
        fscanf(entrada, "%d %d %d", &(graph->edge[i].src), &(graph->edge[i].dest), &(graph->edge[i].weight));
    }

    FILE *saida = fopen(argv[2], "w");

    primMST(graph, saida);
    fclose(entrada);
    fclose(saida);
    return 0;
}
