#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreRubroNegraAtor.h"
#include "grafo.h"



//#define MAX_FIELD_SIZE 1000
#define MAX_LINE_LENGTH 10000

void processarLinhaFilme(char *linha, Grafo * grafo) {

    char* token = strtok(linha, "\t");
    char titulo[BUFSIZ];      
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
            printf(" \tTitulo: %s\t |", titulo);
            break;
        }
        
        token = strtok(NULL, "\t");
        coluna++;
    };
    adicionarFilme(grafo,id,titulo);

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

void lerArquivoTSV(int numLinhas, NodeAtor ** atores, Grafo * grafo) {
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
                processarLinhaFilme(linha, grafo);
                
            }else{
                processarLinhaAtor(linha, atores);
            }
            contador++;
        }

        fclose(arquivo);
        printf("----------------------------------------------------------------------------------------------\n");
    } while (ehFilme == 1);
}


void percorrerArvoreAtor(NodeAtor* raiz, Grafo* grafo) {
    if (raiz == NULL) {
        return;
    }

    // Verificar se o ator possui filmes
    if (raiz->numFilmes > 0) {
        // Verificar se algum filme do ator já está presente no grafo
        int possuiFilmeNoGrafo = 0;
        for (int i = 0; i < raiz->numFilmes; i++) {
            if (buscarFilmeNoGrafo(grafo, raiz->Filmes[i])) {
                possuiFilmeNoGrafo = 1; 
                break;
            }
        }

        // Adicionar uma aresta apenas se o ator tiver um filme no grafo
        if (possuiFilmeNoGrafo) {
            for (int i = 0; i < raiz->numFilmes; i++) {
                adicionarAresta(grafo, raiz->chave, raiz->Filmes[i]);
                printf("%d\n",raiz->Filmes[i]);
            }
        }
    }

    // Percorrer a subárvore esquerda
    percorrerArvoreAtor(raiz->esquerda, grafo);

    // Percorrer a subárvore direita
    percorrerArvoreAtor(raiz->direita, grafo);
}




int main() {
    NodeAtor* atores = NULL;
    Grafo* grafo = criarGrafo();

    lerArquivoTSV(1000, &atores, grafo);

    printf("\n\nAtores:\n");
    // imprimirArvoreAtor(atores);

    percorrerArvoreAtor(atores,grafo);


    printf("\nGrafo:\n");
    imprimirGrafo(grafo);

    visitarGrafoDot(grafo);


    // imprimirArvoreFilme(grafo);


    // imprimirNo(filmes,2);

    liberarArvoreAtor(atores);
    liberarGrafo(grafo);

    return 0;
}

