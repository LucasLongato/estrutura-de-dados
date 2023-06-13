#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreRubroNegra.h"
#include "arvoreRubroNegraFilme.h"



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
            printf("\t%s\t |", nome);
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

int main() {
    NodeAtor* atores = NULL;
    NodeFilme* filmes = NULL;

    lerArquivoTSV(10, &atores, &filmes);

    // imprimirArvore(atores);
    // imprimirArvoreFilme(filmes);


    // imprimirNo(filmes,2);

    liberarArvore(atores);
    liberarArvoreFilme(filmes);

    return 0;
}