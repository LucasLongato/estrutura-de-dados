#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreRubroNegraAtor.h"
#include "arvoreRubroNegraFilme.h"
#include "grafo.h"
//#define MAX_FIELD_SIZE 1000
#define MAX_LINE_LENGTH 10000

typedef struct grafo{
    int * adj;
    int numVertices;
}Grafo;

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

//grafo
void addAresta(Grafo * grafo, NodeFilme * filmesArray, int filme1, int filme2){
    for (grafo == NULL || filme1 < 0 || filme2 < 0 || filme1 >= grafo->numVertices || filme2 >= grafo->numVertices){
        return;
    }
    grafo->adj[filme1 * grafo->numVertices + filme2] = 1;
    grafo->adj[filme2 * grafo->numVertices + filme1] = 1;
}


grafo criargrafo(NodeFilme * filmesArray, int numFilmes){ 
    grafo grafo;
    grafo.numVertices = numFilmes;
    grafo.adj = calloc(numFilmes * numFilmes, sizeof(int));
    if (grafo.adj == NULL) {
        printf("Erro ao alocar memória.\n");
        return grafo;
    }

    for (int i = 0; i < numFilmes; i++) {
        for (int j = i + 1; j < numFilmes; j++) {
            Filme filme1 = filmesArray[i];
            Filme filme2 = filmesArray[j];

            if (filme1.ano == filme2.ano && strcmp(filme1.genero, filme2.genero) == 0) {
                addAresta(&grafo, filmesArray, i, j);
            }
        }
    }

    return grafo;
}


void printGrafo(grafo* grafo, NodeFilme* filmesArray) {
    if (grafo == NULL || filmesArray == NULL) {
        return;
    }

    FILE* file = fopen("grafo.dot", "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo grafo.dot\n");
        return;
    }

    fprintf(file, "grafo G {\n");

    for (int i = 0; i < grafo->numVertices; i++) {
        Filme filme = filmesArray[i];

        fprintf(file, "\t%d [label=\"%s (%d)\"];\n", i, filme.titulo, filme.ano);

        for (int j = 0; j < grafo->numVertices; j++) {
            if (grafo->adj[i * grafo->numVertices + j] == 1) {
                Filme filmeRelacionado = filmesArray[j];
                fprintf(file, "\t%d -- %d;\n", i, j);
            }
        }
    }

    fprintf(file, "}\n");
    fclose(file);

    printf("O arquivo grafo.dot foi gerado com sucesso.\n");
}
    

//_-------------------------grafo--------------------



int main() {
    NodeAtor* atores = NULL;
    NodeFilme* filmes = NULL;
    Filme* filmesArray = NULL;
    int numFilmes = 0;
    

    lerArquivoTSV(50, &atores, &filmes);

    printf("\n\nAtores:\n");
    imprimirArvoreAtor(atores);

    printf("\n\nFilmes:\n");
    imprimirArvoreFilme(filmes);

    printf("\n\nGrafo:\n");
    for (int i = 0; i < numFilmes; i++) {
        Filme filme = filmesArray[i];
        for (int j = 0; j < filme.numAtores; j++) {
            int atorId = filme.atores[j];
            //insert_actor(tree, atorId, actors);
        }
    }
    Grafo *grafo=criarGrafo(filmesArray, numFilmes);
    printGrafo(&grafo, filmes);


    // imprimirNo(filmes,2);

    liberarArvoreAtor(atores);
    liberarArvoreFilme(filmes);

    return 0;
}
