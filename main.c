#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreRubroNegra.h"


//#define MAX_FIELD_SIZE 1000
#define MAX_LINE_LENGTH 10000

void processarLinhaFilme(char *linha, Node ** filmes) {

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
            printf("Id: %d\t", id);
            break;
        case 3: 
            strcpy(titulo, token);
            printf("%s\t", titulo);
            break;
        case 5:
            ano = atoi(token);
            printf("%d\t", ano);
            break;
        case 8:
            strcpy(genero, token);
            printf("%s\t", genero);
            break;
        }
        
        token = strtok(NULL, "\t");
        coluna++;
    };

    *filmes = inserirNoAtor(*filmes, id, titulo, ano, genero);
    printf("\n");
}

void processarLinhaAtor(char *linha,Node ** root) {

    char* token = strtok(linha, "\t");
    char nome[BUFSIZ];
    int coluna = 0;
    int id;
    int* filmesArray = NULL;
    int numOcorrencias;
  

    while (token != NULL) {
        switch (coluna)
        {
        case 0:
            id = atoi(token + 2);
            printf("Id: %d\t", id);
            break;
        case 3: 
            strcpy(nome, token);
            printf("%s\t", nome);
            break;
        case 5:
            numOcorrencias = 0;
            char *titles = token;

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

            for (int i = 0; i < numOcorrencias; i++) {
                printf("%d ", filmesArray[i]);
            }

            *root = inserirNoAtor(*root,id,nome,filmesArray,numOcorrencias);
            free(filmesArray);
            break;
        }
    }
    printf("\n");
}

void lerArquivoTSV(const char *nomeArquivo, int numLinhas,Node ** filmes, Node ** atores) {
    FILE *arquivo;
    char linha[MAX_LINE_LENGTH];
    int contador = 0;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fgets(linha, MAX_LINE_LENGTH, arquivo);

    while (fgets(linha, MAX_LINE_LENGTH, arquivo) != NULL && contador < numLinhas) {
        // processarLinhaAtor(linha,root);
        processarLinhaFilme(linha, filmes);
        contador++;
    }

    fclose(arquivo);
}

int main() {
    Node* filmes = NULL;
    Node* atores = NULL;

    lerArquivoTSV("../dataTitle.tsv", 3, &filmes, &atores);

    printf('\n\n\n');
    imprimirArvore(filmes);

    // imprimirNo(filmes,2);

    liberarArvore(filmes);

    return 0;
}