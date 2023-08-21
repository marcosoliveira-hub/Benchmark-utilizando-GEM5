// Edmonds Karp

// to compile:
// gcc main.c -o main

// to run:
// ./main <numero de vertices>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAX_VERTICES 10000

typedef struct
{
    int num_vertices;
    int num_arestas;
    int adj[MAX_VERTICES][MAX_VERTICES];
    int arestas[MAX_VERTICES][MAX_VERTICES];
    int fluxo_maximo;
    int fonte;
    int sumidouro;
} Rede;

Rede *criar_rede(int num_vertices, int fonte, int sumidouro)
{
    Rede *rede = (Rede *)malloc(sizeof(Rede));
    if (rede == NULL)
    {
        printf("Erro ao alocar memória para a estrutura Rede.\n");
        exit(1);
    }
    rede->num_vertices = num_vertices;
    rede->fluxo_maximo = 0;
    rede->num_arestas = 0;
    rede->fonte = fonte;
    rede->sumidouro = sumidouro;

    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = 0; j < num_vertices; j++)
        {
            rede->adj[i][j] = 0;
            rede->arestas[i][j] = 0;
        }
    }
    return rede;
}

void destruir_rede(Rede *rede)
{
    free(rede);
}

void adicionar_aresta(Rede *rede, int u, int v, int capacidade)
{
    rede->adj[u][v] = 1;
    rede->adj[v][u] = 1;
    rede->arestas[u][v] = capacidade;
    rede->num_arestas++;
}

void remover_aresta(Rede *rede, int u, int v)
{
    rede->adj[u][v] = 0;
    rede->arestas[u][v] = 0;
    rede->num_arestas--;
}

int get_capacidade(Rede *rede, int u, int v)
{
    return rede->arestas[u][v];
}

Rede *criar_rede_residual(Rede *rede, int *fluxo)
{
    Rede *rede_residual = criar_rede(rede->num_vertices, rede->fonte, rede->sumidouro);
    for (int u = 0; u < rede->num_vertices; u++)
    {
        for (int v = 0; v < rede->num_vertices; v++)
        {
            if (rede->adj[u][v] == 1)
            {
                if (rede->arestas[u][v] == 0)
                {
                    remover_aresta(rede, u, v);
                }
                else
                {
                    int capacidade = get_capacidade(rede, u, v) - fluxo[u * rede->num_vertices + v];
                    adicionar_aresta(rede_residual, u, v, capacidade);
                }
            }
        }
    }
    return rede_residual;
}

Rede *criar_rede_aleatoria(int num_vertices)
{
    Rede *rede = criar_rede(num_vertices, 0, num_vertices - 1);
    int visitados[num_vertices];
    for (int i = 0; i < num_vertices; i++)
    {
        visitados[i] = 0;
    }
    int u = 0;
    visitados[u] = 1;
    while (rede->num_arestas < (num_vertices - 2))
    {
        int v = rand() % (num_vertices - 2) + 1;
        if (visitados[v] == 0)
        {
            int capacidade = (rand() % 20) + 1;
            adicionar_aresta(rede, u, v, capacidade);
            visitados[v] = 1;
            u = v;
        }
    }
    long int numero_arestas_adicionais = rand() % ((num_vertices * num_vertices) / 4);
    while (numero_arestas_adicionais > 0)
    {
        int verticeOrigem = rand() % (num_vertices - 2);
        int verticeDestino = rand() % (num_vertices - 1) + 1;
        if (rede->arestas[verticeOrigem][verticeDestino] == 0 && verticeDestino != verticeOrigem && rede->adj[verticeOrigem][verticeDestino] == 0)
        {
            int capacidade = (rand() % 20) + 1;
            adicionar_aresta(rede, verticeOrigem, verticeDestino, capacidade);
        }
        numero_arestas_adicionais--;
    }
    int capacidade = (rand() % 20) + 1;
    adicionar_aresta(rede, u, num_vertices - 1, capacidade);
    return rede;
}

// Create a function to write the graph to a file

void escrever_grafo(Rede *rede, char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "w");
    fprintf(arquivo, "%d %d\n", rede->num_vertices, rede->num_arestas);
    for (int i = 0; i < rede->num_vertices; i++)
    {
        for (int j = 0; j < rede->num_vertices; j++)
        {
            if (rede->adj[i][j] == 1 && rede->arestas[i][j] != 0)
            {
                fprintf(arquivo, "%d %d %d\n", i, j, rede->arestas[i][j]);
            }
        }
    }
    fclose(arquivo);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (argc != 3)
    {
        printf("Formato: main <numero de vertices> <arq saida>\n");
        exit(1);
    }

    Rede *rede1 = criar_rede_aleatoria(atoi(argv[1]));
    escrever_grafo(rede1, argv[2]);
    destruir_rede(rede1);
    return 0;
}
