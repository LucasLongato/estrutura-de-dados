#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreRubroNegraAtor.h"
#include "arvoreRubroNegraFilme.h"
#include "grafo.h"


//#define MAX_FIELD_SIZE 1000
#define MAX_LINE_LENGTH 10000

void processarLinhaFilme(char *linha, NodeFilme ** filmes) {

    char* token = strtok(linha, "\t");
    char titulo[BUFSIZ]; //Coluna 3
    char genero[BUFSIZ]; //Coluna 8
    int ano;             //Coluna 0
    int coluna = 0;
    int id;
  

    while (token != NULL) {
        switch (coluna)
        {
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
    };

    *filmes = inserirNoFilme(*filmes, id, titulo, genero, ano);
    printf("\n");
}

void processarLinhaAtor(char *linha, NodeAtor ** root) {

    char* token = strtok(linha, "\t");
    char nome[BUFSIZ];
    int coluna = 0;
    int id;
    int* filmesArray = NULL;
  

    while (token != NULL) {
        int numOcorrencias;
        char *titles;
        switch (coluna)
        {
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

            *root = inserirNoAtor(*root,id,nome,filmesArray,numOcorrencias);
            free(filmesArray);
        }
        
        token = strtok(NULL, "\t");
        coluna++;
    }

    printf("\n");
}

void lerArquivoTSV(int numLinhas, NodeAtor ** atores, NodeFilme ** filmes) {
    int ehFilme = 0;
    FILE *arquivo;

    do
    {   
        char linha[MAX_LINE_LENGTH];
        int contador = 0;
        ehFilme++;

        if(ehFilme == 1){
            arquivo = fopen("dataTitle.tsv", "r");
        }else{
            arquivo = fopen("dataName.tsv", "r");
        }

        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo.\n");

            return;
        }

        fgets(linha, MAX_LINE_LENGTH, arquivo);

        while (fgets(linha, MAX_LINE_LENGTH, arquivo) != NULL && contador < numLinhas) {
            if(ehFilme == 1){
                processarLinhaFilme(linha, filmes);
            }else{
                processarLinhaAtor(linha, atores);
            }
            contador++;
        }

        fclose(arquivo);
        printf("----------------------------------------------------------------------------------------------\n");
    } while (ehFilme == 1);
}

///GRAFOS

void addEdge(Grafo* grafo, filme* filme int movieIndex1, int movieIndex2) {
    if (grafo == NULL || movieIndex1 < 0 || movieIndex2 < 0 ||
        movieIndex1 >= grafo->numVertices || movieIndex2 >= grafo->numVertices) {
        return;
    }

    grafo->adjMatrix[movieIndex1 * grafo->numVertices + movieIndex2] = 1;
    grafo->adjMatrix[movieIndex2 * grafo->numVertices + movieIndex1] = 1;
}


grafh criargrafo(filme* filmes, int numfilmes) {
    grafh grafo;
    grafo.numVertices = numfilmes;
    grafo.adjMatrix = calloc(numfilmes * numfilmes, sizeof(int));
    if (grafo.adjMatrix == NULL) {
        printf("Erro ao alocar mem ria.\n");
        return grafo;
    }

    for (int i = 0; i < numfilmes; i++) {
        for (int j = i + 1; j < numfilmes; j++) {
            filme filme1 = filmes[i];
            filme filme2 = filmes[j];

            if (filme1.startYear == filme2.startYear && strcmp(filme1.genres, filme2.genres) == 0) {
                addEdge(&grafo, filmes, i, j);
            }
        }
    }

    return grafo;
}


void grafoprint(grafh grafo, filme* filmes) {
    if (grafo == NULL || filmes == NULL) {
        return;
    }

    for (int i = 0; i < grafo.numVertices; i++) {
        filme filme = filmes[i];

        printf("%s (%d):\n", filme.primaryTitle, filme.startYear);

        for (int j = 0; j < grafo.numVertices; j++) {
            if (grafo.adjMatrix[i * grafo.numVertices + j] == 1) {
                filme relatedMovie = filmes[j];
                printf("\t%s (%d)\n", relatedMovie.primaryTitle, relatedMovie.startYear);
            }
        }

        printf("\n");
    }
}

//______________________________________________
  grafoDot(&grafo, filmes, "grafo.dot"){

    fprintf(arquivo, "graph {\n");

    for (int i = 0; i < grafo.numVertices; i++) {
        filme filme = filmes[i];
        fprintf(arquivo, "\t%d [label=\"%s (%d)\"];\n", i, filme.primaryTitle, filme.startYear);
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
//______________________________________________
int main() {
    NodeAtor* atores = NULL;
    NodeFilme* filmes = NULL;

    lerArquivoTSV(50, &atores, &filmes);

    printf("\n\nAtores:\n");
    imprimirArvoreAtor(atores);

    printf("\n\nFilmes:\n");
    imprimirArvoreFilme(filmes);

    //graph
    
    printf("\n\nGrafo:\n");
    Grafo grafo =criargrafo(filmes, 50);
    grafoprint(grafo, filmes);

   



    // imprimirNo(filmes,2);

    liberarArvoreAtor(atores);
    liberarArvoreFilme(filmes);


    return 0;
}


