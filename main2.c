#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreRubroNegra.h"


//#define MAX_FIELD_SIZE 1000
#define MAX_LINE_LENGTH 10000

void processarLinha(char *linha,Node ** root) {

    char* token = strtok(linha, "\t");
    char nome[BUFSIZ];
    int coluna = 0;
    int id;
    int* filmesArray = NULL;
  

    while (token != NULL) {
        if (coluna == 0) {
            id = atoi(token + 2);
            printf("Id: %d\t", id);
        }
        else if (coluna == 1) {
            strcpy(nome, token);
            printf("%s\t", nome);
        }
        else if (coluna == 5) {

            
            int numOcorrencias = 0;
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

            *root = inserirNo(*root,id,nome,filmesArray,numOcorrencias);
            free(filmesArray);
        }
        
        token = strtok(NULL, "\t");
        coluna++;
    }

    printf("\n");
}

void lerArquivoTSV(const char *nomeArquivo, int numLinhas,Node ** root) {
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
        processarLinha(linha,root);
        contador++;
    }

    fclose(arquivo);
}



int main() {
    Node* root = NULL;

    lerArquivoTSV("datanome.tsv", 5,&root);

    imprimirArvore(root);


    imprimirNo(root,2);

    liberarArvore(root);

    artists_file = fopen("datanome,tsv", "r");
    movies_file = fopen("data.tsv", "r");

    read_artists(artists_file, &artists);
    read_movies(movies_file, &movies);

    build_graph(artists, movies, &graph);

    print_graph_dot(graph);

    return 0;
}

