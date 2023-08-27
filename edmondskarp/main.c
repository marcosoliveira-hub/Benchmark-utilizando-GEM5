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

// Estrutura de dados para representar uma rede de fluxo
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

// Função para criar uma estrutura Rede

Rede *criar_rede(int num_vertices, int num_arestas, int fonte, int sumidouro)
{
    Rede *rede = (Rede *)malloc(sizeof(Rede));
    if (rede == NULL)
    {
        printf("Erro ao alocar memória para a estrutura Rede.\n");
        exit(1);
    }
    rede->num_vertices = num_vertices;
    rede->fluxo_maximo = 0;
    rede->num_arestas = num_arestas;
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

// Função para destruir uma estrutura Rede
void destruir_rede(Rede *rede)
{
    free(rede);
}

// Função para adicionar uma aresta em uma estrutura Rede
void adicionar_aresta(Rede *rede, int u, int v, int capacidade)
{
    rede->adj[u][v] = 1;
    rede->adj[v][u] = 1;
    rede->arestas[u][v] = capacidade;
    rede->num_arestas++;
}

//  Função para remover uma aresta em uma estrutura Rede
void remover_aresta(Rede *rede, int u, int v)
{
    rede->adj[u][v] = 0;
    rede->arestas[u][v] = 0;
    rede->num_arestas--;
}

// Função para obter a capacidade de uma aresta em uma estrutura Rede
int get_capacidade(Rede *rede, int u, int v)
{
    return rede->arestas[u][v];
}

// Função para mostrar as arestas de uma estrutura Rede
void mostrar_arestas(Rede *rede)
{
    for (int i = 0; i < rede->num_vertices; i++)
    {
        for (int j = 0; j < rede->num_vertices; j++)
        {
            if (rede->adj[i][j] == 1)
            {
                printf("Aresta (%d, %d), Capacidade: %d\n", i, j, rede->arestas[i][j]);
            }
        }
    }
}

// Função para executar uma busca em largura em uma estrutura Rede
int *bfs(Rede *rede, int vertice_inicial)
{
    int *visitado = (int *)malloc(rede->num_vertices * sizeof(int));

    if (visitado == NULL)
    {
        printf("Erro ao alocar memória para o vetor visitado.\n");
        exit(1);
    }

    int *pai = (int *)malloc(rede->num_vertices * sizeof(int));

    if (pai == NULL)
    {
        printf("Erro ao alocar memória para o vetor pai.\n");
        exit(1);
    }

    int fila[rede->num_vertices];
    int inicio = 0;
    int fim = 0;
    for (int i = 0; i < rede->num_vertices; i++)
    {
        visitado[i] = 0;
        pai[i] = -1;
    }
    fila[fim++] = vertice_inicial;
    visitado[vertice_inicial] = 1;
    pai[vertice_inicial] = -1;
    while (inicio != fim)
    {
        int u = fila[inicio++];
        for (int v = 0; v < rede->num_vertices; v++)
        {
            if (visitado[v] == 0 && get_capacidade(rede, u, v) > 0)
            {
                fila[fim++] = v;
                visitado[v] = 1;
                pai[v] = u;
            }
        }
    }

    free(visitado);

    return pai;
}

// Função para criar uma rede residual a partir de uma rede e um vetor de fluxo
Rede *criar_rede_residual(Rede *rede, int *fluxo)
{
    Rede *rede_residual = criar_rede(rede->num_vertices, rede->num_arestas, rede->fonte, rede->sumidouro);
    for (int u = 0; u < rede->num_vertices; u++)
    {
        for (int v = 0; v < rede->num_vertices; v++)
        {
            if (rede->adj[u][v] == 1)
            {
                int capacidade = get_capacidade(rede, u, v) - fluxo[u * rede->num_vertices + v];
                adicionar_aresta(rede_residual, u, v, capacidade);
            }
        }
    }
    return rede_residual;
}

// Função para escrever uma rede em um arquivo
void escrever_fluxo_maximo_e_grafo(Rede *rede, int fluxo_maximo, char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "w");
    fprintf(arquivo, "Fluxo maximo = %d\n", fluxo_maximo);
    int num_arestas = rede->num_arestas;
    int num_vertices = rede->num_vertices;
    fprintf(arquivo, "%d %d\n", num_vertices, num_arestas);
    for (int i = 0; i < rede->num_vertices; i++)
    {
        for (int j = 0; j < rede->num_vertices; j++)
        {
            if (rede->adj[i][j] == 1)
            {
                fprintf(arquivo, "%d %d %d\n", i, j, rede->arestas[i][j]);
            }
        }
    }
    fclose(arquivo);
}

// Função para executar o algoritmo de Edmonds Karp em uma estrutura Rede
int edmonds_karp(Rede *rede)
{
    rede->fluxo_maximo = 0;
    int *fluxo = (int *)malloc(rede->num_vertices * rede->num_vertices * sizeof(int));

    if (fluxo == NULL)
    {
        printf("Erro ao alocar memória para o vetor fluxo.\n");
        exit(1);
    }

    for (int i = 0; i < rede->num_vertices * rede->num_vertices; i++)
    {
        fluxo[i] = 0;
    }
    Rede *rede_residual = criar_rede_residual(rede, fluxo);
    int *pai = bfs(rede_residual, rede->fonte);
    while (pai[rede->sumidouro] != -1)
    {
        int fluxo_caminho = INT_MAX;
        int v = rede->sumidouro;
        while (v != rede->fonte)
        {
            int u = pai[v];
            fluxo_caminho = fluxo_caminho < get_capacidade(rede_residual, u, v) ? fluxo_caminho : get_capacidade(rede_residual, u, v);
            v = u;
        }
        v = rede->sumidouro;
        while (v != rede->fonte)
        {
            int u = pai[v];
            if (rede->adj[u][v] == 1)
            {
                fluxo[u * rede->num_vertices + v] += fluxo_caminho;
            }
            else if (rede->adj[v][u] == 1)
            {
                fluxo[v * rede->num_vertices + u] -= fluxo_caminho;
            }
            v = u;
        }
        rede->fluxo_maximo += fluxo_caminho;
        rede_residual = criar_rede_residual(rede, fluxo);
        pai = bfs(rede_residual, rede->fonte);
    }

    for (int i = 0; i < rede->num_vertices; i++)
    {
        for (int j = 0; j < rede->num_vertices; j++)
        {
            if (rede->adj[i][j] == 1)
            {
                if (rede->arestas[i][j] == 0)
                {
                    remover_aresta(rede, i, j);
                }
            }
        }
    }

    destruir_rede(rede_residual);
    free(fluxo);
    free(pai);

    return rede->fluxo_maximo;
}

// Função para criar uma rede a partir de um arquivo

Rede *criar_rede_arquivo(char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo %s.\n", nome_arquivo);
        exit(1);
    }
    int num_vertices;
    int num_arestas;
    fscanf(arquivo, "%d %d", &num_vertices, &num_arestas);
    Rede *rede = criar_rede(num_vertices, num_arestas, 0, num_vertices - 1);
    int u, v, capacidade;
    while (fscanf(arquivo, "%d %d %d", &u, &v, &capacidade) != EOF)
    {
        adicionar_aresta(rede, u, v, capacidade);
    }
    fclose(arquivo);
    return rede;
}

// Função principal
int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (argc != 3)
    {
        printf("exec: main <arq entrada> <arq saida>\n");
        exit(1);
    }

    Rede *rede = criar_rede_arquivo(argv[1]);
    int fluxo_maximo = edmonds_karp(rede);
    escrever_fluxo_maximo_e_grafo(rede, fluxo_maximo, argv[2]);
    destruir_rede(rede);
    return 0;
}
