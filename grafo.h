#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structs para o grafo
typedef struct {
    char* titleId;
    char* titleType;
    char* primaryTitle;
    char* originalTitle;
    int isAdult;
    int startYear;
    int endYear;
    int runtimeMinutes;
    char* genres;
    int* artists; // Array of artist IDs associated with the movie
    int numArtists; // Number of artists associated with the movie
} filmes;

typedef struct {
    int ID;
    char* nome;
    char** movies;
    int numMovies;
} Artistas;

typedef struct {
    int* adjMatrix;
    int numVertices;
} Grafo;


void addAresta(Grafo * grafo, NodeFilme * filmesArray, int filme1, int filme2);
grafo criargrafo(NodeFilme * filmesArray, int numFilmes);
void printGrafo(grafo* grafo, NodeFilme* filmesArray);
