#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Aresta {
    int indice;
    struct Aresta* proxima;
} Aresta;

typedef struct {
    int id;
    char nome[BUFSIZ];
    int posicao;
    Aresta* arestas;
} Filme;

typedef struct {
    Filme* filmes;
    int tamanho;
    int capacidade;
} ListaFilmes;

typedef struct {
    ListaFilmes listaFilmes;
} Grafo;

Grafo* criarGrafo() {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->listaFilmes.filmes = NULL;
    grafo->listaFilmes.tamanho = 0;
    grafo->listaFilmes.capacidade = 0;
    return grafo;
}

void adicionarFilme(Grafo* grafo, int id, char nome[]) {
    if (grafo == NULL) {
        return;
    }

    // Verificar se o filme já existe no grafo
    int i;
    for (i = 0; i < grafo->listaFilmes.tamanho; i++) {
        if (grafo->listaFilmes.filmes[i].id == id) {
            return;
        }
    }

    // Aumentar a capacidade do vetor de filmes se necessário
    if (grafo->listaFilmes.tamanho == grafo->listaFilmes.capacidade) {
        int novaCapacidade = (grafo->listaFilmes.capacidade == 0) ? 1 : grafo->listaFilmes.capacidade * 2;
        Filme* novoVetor = (Filme*)realloc(grafo->listaFilmes.filmes, novaCapacidade * sizeof(Filme));
        if (novoVetor == NULL) {
            return;
        }
        grafo->listaFilmes.filmes = novoVetor;
        grafo->listaFilmes.capacidade = novaCapacidade;
    }

    // Adicionar o novo filme ao vetor
    Filme novoFilme;
    novoFilme.id = id;
    strcpy(novoFilme.nome, nome);
    novoFilme.posicao = grafo->listaFilmes.tamanho;
    novoFilme.arestas = NULL;
    grafo->listaFilmes.filmes[grafo->listaFilmes.tamanho] = novoFilme;
    grafo->listaFilmes.tamanho++;

}

void adicionarAresta(Grafo* grafo, int idOrigem, int idDestino) {
    if (grafo == NULL) {
        return;
    }

    // Encontrar os filmes correspondentes aos IDs de origem e destino
    Filme* filmeOrigem = NULL;
    Filme* filmeDestino = NULL;
    int i;
    for (i = 0; i < grafo->listaFilmes.tamanho; i++) {
        if (grafo->listaFilmes.filmes[i].id == idOrigem) {
            filmeOrigem = &grafo->listaFilmes.filmes[i];
        }
        if (grafo->listaFilmes.filmes[i].id == idDestino) {
            filmeDestino = &grafo->listaFilmes.filmes[i];
        }
    }

    // Verificar se os filmes foram encontrados
    if (filmeOrigem == NULL || filmeDestino == NULL) {
        return;
    }
    printf("Origem: %d Destino: %d\n",filmeOrigem,filmeDestino);
    // Criar uma nova aresta
    Aresta* novaAresta = (Aresta*)malloc(sizeof(Aresta));
    novaAresta->indice = filmeDestino->posicao;
    novaAresta->proxima = NULL;

    // Adicionar a nova aresta à lista de arestas do filme de origem
    Aresta* arestaAtual = filmeOrigem->arestas;
    if (arestaAtual == NULL) {
        filmeOrigem->arestas = novaAresta;
    } else {
        while (arestaAtual->proxima != NULL) {
            arestaAtual = arestaAtual->proxima;
        }
        arestaAtual->proxima = novaAresta;
    }
}

int buscarFilmeNoGrafo(Grafo* grafo, int idFilme) {
    if (grafo == NULL) {
        return 0;
    }
    int i;
    for (i = 0; i < grafo->listaFilmes.tamanho; i++) {
        if (grafo->listaFilmes.filmes[i].id == idFilme) {
            return 1;
        }
    }
    return 0;
}

// void imprimirGrafo(Grafo* grafo) {
//     if (grafo == NULL) {
//         return;
//     }

//     int i, j;
//     for (i = 0; i < grafo->listaFilmes.tamanho; i++) {
//         Filme filme = grafo->listaFilmes.filmes[i];
//         printf("Filme %d: %s\n", filme.id, filme.nome);
//         printf("Conexoes: ");
//         Aresta* arestaAtual = filme.arestas;
//         while (arestaAtual != NULL) {
//             Filme filmeConexao = grafo->listaFilmes.filmes[arestaAtual->indice];
//             printf("%s ", filmeConexao.nome);
//             arestaAtual = arestaAtual->proxima;
//         }
//         printf("\n\n");
//     }
// }

void imprimirGrafo(Grafo* grafo) {
    if (grafo == NULL) {
        return;
    }

    int i, j;
    for (i = 0; i < grafo->listaFilmes.tamanho; i++) {
        Filme filme = grafo->listaFilmes.filmes[i];

        if (filme.arestas != NULL) {
            printf("Filme %d: %s\n", filme.id, filme.nome);
            printf("Conexoes (%d): ", contarConexoes(&filme));

            Aresta* arestaAtual = filme.arestas;
            while (arestaAtual != NULL) {
                Filme filmeConexao = grafo->listaFilmes.filmes[arestaAtual->indice];
                printf("%s", filmeConexao.nome);

                if (arestaAtual->proxima != NULL) {
                    printf(" - ");
                }

                arestaAtual = arestaAtual->proxima;
            }

            printf("\n\n");
        }
    }
}

int contarConexoes(Filme* filme) {
    int count = 0;
    Aresta* arestaAtual = filme->arestas;
    while (arestaAtual != NULL) {
        count++;
        arestaAtual = arestaAtual->proxima;
    }
    return count;
}

void liberarGrafo(Grafo* grafo) {
    if (grafo == NULL) {
        return;
    }

    int i;
    for (i = 0; i < grafo->listaFilmes.tamanho; i++) {
        Aresta* arestaAtual = grafo->listaFilmes.filmes[i].arestas;
        while (arestaAtual != NULL) {
            Aresta* proximaAresta = arestaAtual->proxima;
            free(arestaAtual);
            arestaAtual = proximaAresta;
        }
    }

    free(grafo->listaFilmes.filmes);
    free(grafo);
}

void visitarGrafoDot(Grafo* grafo) {
    if (grafo == NULL) {
        return;
    }

    printf("graph {\n");

    int i, j;
    for (i = 0; i < grafo->listaFilmes.tamanho; i++) {
        Filme filme = grafo->listaFilmes.filmes[i];
        Aresta* arestaAtual = filme.arestas;
        while (arestaAtual != NULL) {
            Filme filmeConexao = grafo->listaFilmes.filmes[arestaAtual->indice];
            printf("\"%s\" -- \"%s\";\n", filme.nome, filmeConexao.nome);
            arestaAtual = arestaAtual->proxima;
        }
    }

    printf("}\n");
}




