#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_V 5000

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Formato: ./generateGraphFile <number of vertices> <output file>\n");
        exit(1);
    }

    if (atoi(argv[1]) > MAX_V)
    {
        printf("Number of vertices must be less than %d\n", MAX_V);
        exit(1);
    }

    srand(time(NULL));

    int V = atoi(argv[1]);
    long long int E;

    FILE *file = fopen(argv[2], "w");
    fprintf(file, "%d ", V);

    E = V * (V - 1) / 2;

    fprintf(file, "%lld\n", E);

    int src, dest, weight;

    int **edges = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++)
    {
        edges[i] = (int *)malloc(V * sizeof(int));
        memset(edges[i], 0, V * sizeof(int));
    }

    for (int i = 0; i < E; i++)
    {
        src = rand() % V;
        dest = rand() % V;
        weight = rand() % 100 + 1;

        if (edges[src][dest] == 0 && src != dest)
        {
            fprintf(file, "%d %d %d\n", src, dest, weight);
            edges[src][dest] = 1;
            edges[dest][src] = 1;
        }
        else
        {
            i--;
        }
    }

    for (int i = 0; i < V; i++)
    {
        free(edges[i]);
    }
    free(edges);

    fclose(file);
    return 0;
}