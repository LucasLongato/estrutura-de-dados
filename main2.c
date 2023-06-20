#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreRubroNegraAtor.h"
#include "arvoreRubroNegraFilme.h"
#include "grafo.h"

#define MAX_LINE_LENGTH 10000

void processarLinhaFilme(char *linha, NodeFilme **filmes) {
    char *token = strtok(linha, "\t");
    char titulo[BUFSIZ]; // Column 3
    char genero[BUFSIZ]; // Column 8
    int ano;             // Column 5
    int coluna = 0;
    int id;

    while (token != NULL) {
        switch (coluna) {
            case 0:
                id = atoi(token + 2);
                printf("| \tId: %d\t |", id);
                break;
            case 3:
                strcpy(titulo, token);
                printf(" \t%s\t |", titulo);
                break;
            case 5:
                ano = atoi(token);
                printf(" \t%d\t |", ano);
                break;
            case 8:
                strcpy(genero, token);
                printf(" \t%s|", genero);
                break;
        }

        token = strtok(NULL, "\t");
        coluna++;
    }

    *filmes = inserirNoFilme(*filmes, id, titulo, genero, ano);
    printf("\n");
}

void processarLinhaAtor(char *linha, NodeAtor **root) {
    char *token = strtok(linha, "\t");
    char nome[BUFSIZ];
    int coluna = 0;
    int id;
    int *filmesArray = NULL;

    while (token != NULL) {
        int numOcorrencias;
        char *titles;
        switch (coluna) {
            case 0:
                id = atoi(token + 2);
                printf("| \tId: %d\t |", id);
                break;
            case 1:
                strcpy(nome, token);
                printf("\t%s\t\t |", nome);
                break;
            case 5:
                numOcorrencias = 0;
                titles = token;

                while ((titles = strstr(titles, "tt")) != NULL) {
                    numOcorrencias++;
                    titles++;
                }

                filmesArray = malloc(numOcorrencias * sizeof(int));
                titles = strtok(token, ",");

                for (int i = 0; i < numOcorrencias; i++) {
                    filmesArray[i] = strtol(titles + 2, NULL, 10);
                    titles = strtok(NULL, ",");
                }

                printf(" \t[");
                for (int i = 0; i < numOcorrencias; i++) {
                    printf("%d ", filmesArray[i]);
                }
                printf("]\t |");

                *root = inserirNoAtor(*root, id, nome, filmesArray, numOcorrencias);
                free(filmesArray);
        }

        token = strtok(NULL, "\t");
        coluna++;
    }

    printf("\n");
}

void lerArquivoTSV(int numLinhas, NodeAtor **atores, NodeFilme **filmes) {
    int ehFilme = 0;
    FILE *arquivo;

    do {
        char linha[MAX_LINE_LENGTH];
        int contador = 0;
        ehFilme++;

        if (ehFilme == 1) {
            arquivo = fopen("dataTitle.tsv", "r");
        } else {
            arquivo = fopen("dataName.tsv", "r");
        }

        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return;
        }

        fgets(linha, MAX_LINE_LENGTH, arquivo);

        while (fgets(linha, MAX_LINE_LENGTH, arquivo) != NULL && contador < numLinhas) {
            if (ehFilme == 1) {
                processarLinhaFilme(linha, filmes);
            } else {
                processarLinhaAtor(linha, atores);
            }
            contador++;
        }

        fclose(arquivo);
        printf("----------------------------------------------------------------------------------------------\n");
    } while (ehFilme == 1);
}

// GRAFOS

void addEdge(Grafo *grafo, int movieIndex1, int movieIndex2) {
    if (grafo == NULL || movieIndex1 < 0 || movieIndex2 < 0 ||
        movieIndex1 >= grafo->numVertices || movieIndex2 >= grafo->numVertices) {
        return;
    }

    grafo->adjMatrix[movieIndex1 * grafo->numVertices + movieIndex2] = 1;
    grafo->adjMatrix[movieIndex2 * grafo->numVertices + movieIndex1] = 1;
}

Grafo criarGrafo(NodeFilme *filmes, int numFilmes) {
    Grafo grafo;
    grafo.numVertices = numFilmes;
    grafo.adjMatrix = calloc(numFilmes * numFilmes, sizeof(int));

    if (grafo.adjMatrix == NULL) {
        printf("Erro ao alocar memória.\n");
        return grafo;
    }

    for (int i = 0; i < numFilmes; i++) {
        for (int j = i + 1; j < numFilmes; j++) {
            NodeFilme *filme1 = buscarFilme(filmes, i);
            NodeFilme *filme2 = buscarFilme(filmes, j);

            if (filme1 != NULL && filme2 != NULL && filme1->ano == filme2->ano &&
                strcmp(filme1->genero, filme2->genero) == 0) {
                addEdge(&grafo, i, j);
            }
        }
    }

    return grafo;
}

void imprimirGrafo(Grafo grafo, NodeFilme *filmes) {
    if (grafo.adjMatrix == NULL || filmes == NULL) {
        return;
    }

    for (int i = 0; i < grafo.numVertices; i++) {
        NodeFilme *filme = buscarFilme(filmes, i);

        if (filme != NULL) {
            printf("%s (%d):\n", filme->titulo, filme->ano);

            for (int j = 0; j < grafo.numVertices; j++) {
                if (grafo.adjMatrix[i * grafo.numVertices + j] == 1) {
                    NodeFilme *relatedMovie = buscarFilme(filmes, j);
                    if (relatedMovie != NULL) {
                        printf("\t%s (%d)\n", relatedMovie->titulo, relatedMovie->ano);
                    }
                }
            }

            printf("\n");
        }
    }
}

void gerarGrafoDot(Grafo grafo, NodeFilme *filmes, const char *filename) {
    FILE *arquivo = fopen(filename, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    fprintf(arquivo, "graph {\n");

    for (int i = 0; i < grafo.numVertices; i++) {
        NodeFilme *filme = buscarFilme(filmes, i);
        if (filme != NULL) {
            fprintf(arquivo, "\t%d [label=\"%s (%d)\"];\n", i, filme->titulo, filme->ano);
        }
    }

    for (int i = 0; i < grafo.numVertices; i++) {
        for (int j = i + 1; j < grafo.numVertices; j++) {
            if (grafo.adjMatrix[i * grafo.numVertices + j] == 1) {
                fprintf(arquivo, "\t%d -- %d;\n", i, j);
            }
        }
    }

    fprintf(arquivo, "}");

    fclose(arquivo);
    printf("Arquivo gerado com sucesso.\n");
}

int main() {
    NodeAtor *atores = NULL;
    NodeFilme *filmes = NULL;

    lerArquivoTSV(50, &atores, &filmes);

    printf("\n\nAtores:\n");
    imprimirArvoreAtor(atores);

    printf("\n\nFilmes:\n");
    imprimirArvoreFilme(filmes);

    printf("\n\nGrafo:\n");
    Grafo grafo = criarGrafo(filmes, 50);
    imprimirGrafo(grafo, filmes);

    gerarGrafoDot(grafo, filmes, "grafo.dot");

    liberarArvoreAtor(atores);
    liberarArvoreFilme(filmes);

    return 0;
}
