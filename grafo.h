#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structs para o grafo
struct artista {
  int id;
  char *nome;
  char **filmes;
  int num_filmes;
}artistas;

struct filme {
  int id;
  char *nome;
  int num_artistas;
};

struct grafo {
  struct filme **vertices;
  int num_vertices;
}grafo;

// funçao para ler o arquivo e criar os vetores de artistas e filmes
void ler_artistas(FILE *arquivo, struct artista **artistas) {
  int i, num_artistas;
  char linha[1024];

  fscanf(arquivo, "%d", &num_artistas);
  *artistas = malloc(sizeof(struct artista) * num_artistas);

  for (i = 0; i < num_artistas; i++) {
    fgets(linha, sizeof(linha), arquivo);
    sscanf(linha, "%d %[^\n]", &(*artistas)[i].id, (*artistas)[i].nome);
    (*artistas)[i].filmes = NULL;
    (*artistas)[i].num_filmes = 0;
  }
}

void ler_filmes(FILE *arquivo, struct filme **filmes) {
  int i, num_filmes;
  char linha[1024];

  fscanf(arquivo, "%d", &num_filmes);
  *filmes = malloc(sizeof(struct filme) * num_filmes);

  for (i = 0; i < num_filmes; i++) {
    fgets(linha, sizeof(linha), arquivo);
    sscanf(linha, "%d %[^\n]", &(*filmes)[i].id, (*filmes)[i].nome);
    (*filmes)[i].num_artistas = 0;
  }
}

// funçao para adicionar um filme ao artista
void adicionar_filme_ao_artista(struct artista *artista, struct filme *filme) {
  artista->filmes = realloc(artista->filmes, sizeof(char *) * (artista->num_filmes + 1));
  artista->filmes[artista->num_filmes++] = filme->nome;
}

// funçao para adicionar um artista ao filme
void adicionar_artista_ao_filme(struct filme *filme, struct artista *artista) {
  filme->num_artistas++;
}

// funçao para construir o grafo
void construir_grafo(struct artista **artistas, struct filme **filmes, struct grafo **grafo) {
  int i, j;

  *grafo = malloc(sizeof(struct grafo));
  (*grafo)->vertices = malloc(sizeof(struct filme *) * (*filmes)->num_filmes);
  (*grafo)->num_vertices = (*filmes)->num_filmes;

  for (i = 0; i < (*filmes)->num_filmes; i++) {
    (*grafo)->vertices[i] = filmes[i];
    for (j = 0; j < (*artistas)->num_filmes; j++) {
      if (strstr(filmes[i].nome, artistas[j].nome) != NULL) {
        adicionar_filme_ao_artista(artistas[j], filmes[i]);
        adicionar_artista_ao_filme(filmes[i], artistas[j]);
      }
    }
  }
}

// funçao para imprimir o grafo
void imprimir_grafo_dot(struct grafo *grafo) {
  int i, j;

  printf("graph {\n");
  for (i = 0; i < grafo->num_vertices; i++) {
    for (j = 0; j < grafo->vertices[i]->num_artistas; j++) {
      printf("\t%s -- %s;\n", grafo->vertices[i]->nome, artistas[j].nome);
    }
  }
  printf("}\n");
}
