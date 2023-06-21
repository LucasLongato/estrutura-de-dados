#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Estrutura de um nó
typedef struct NodeAtor {
    int chave;
    char Ator[BUFSIZ];
    int isRed; // 0 = red | 1 = black
    struct NodeAtor* pai;
    struct NodeAtor* esquerda;
    struct NodeAtor* direita;
    int numFilmes;
    int* Filmes;
} NodeAtor;


NodeAtor* criarNoAtor(int chave, const char* nomeAtor) {
    NodeAtor* novoNo = (NodeAtor*)malloc(sizeof(NodeAtor));
    novoNo->chave = chave;
    strcpy(novoNo->Ator, nomeAtor);
    novoNo->numFilmes = 0;
    novoNo->isRed = 0;
    novoNo->pai = NULL;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->Filmes = NULL;
    return novoNo;
}


void liberarNoAtor(NodeAtor* no) {
    if (no != NULL) {
        free(no->Filmes);
        free(no);
    }
}


void liberarArvoreAtor(NodeAtor* raiz) {
    if (raiz != NULL) {
        liberarArvoreAtor(raiz->esquerda);
        liberarArvoreAtor(raiz->direita);
        liberarNoAtor(raiz);
    }
}

void imprimirArvoreAtor(NodeAtor* raiz) {
    if (raiz != NULL) {
        imprimirArvoreAtor(raiz->esquerda);
        printf("\n| Chave: %d | Nome: %s | ", raiz->chave, raiz->Ator);
        printf(" [");
        for (int i = 0; i < raiz->numFilmes; i++) {
            printf("%d ", raiz->Filmes[i]);
        }
        printf("]");

        imprimirArvoreAtor(raiz->direita);
    }
}

void buscarFilmes(NodeAtor* raiz) {
    if (raiz != NULL) {
        imprimirArvoreAtor(raiz->esquerda);
        printf("\n| Chave: %d | Nome: %s | ", raiz->chave, raiz->Ator);
        printf(" [");
        for (int i = 0; i < raiz->numFilmes; i++) {
            printf("%d ", raiz->Filmes[i]);
        }
        printf("]");

        imprimirArvoreAtor(raiz->direita);
    }
}




NodeAtor* rotacionarEsquerdaAtor(NodeAtor* raiz, NodeAtor* no) {
    NodeAtor* filhoDireita = no->direita;
    no->direita = filhoDireita->esquerda;
    if (filhoDireita->esquerda != NULL) {
        filhoDireita->esquerda->pai = no;
    }
    filhoDireita->pai = no->pai;
    if (no->pai == NULL) {
        raiz = filhoDireita;
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = filhoDireita;
    } else {
        no->pai->direita = filhoDireita;
    }
    filhoDireita->esquerda = no;
    no->pai = filhoDireita;
    return raiz;
}


NodeAtor* rotacionarDireitaAtor(NodeAtor* raiz, NodeAtor* no) {
    NodeAtor* filhoEsquerda = no->esquerda;
    no->esquerda = filhoEsquerda->direita;
    if (filhoEsquerda->direita != NULL) {
        filhoEsquerda->direita->pai = no;
    }
    filhoEsquerda->pai = no->pai;
    if (no->pai == NULL) {
        raiz = filhoEsquerda;
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = filhoEsquerda;
    } else {
        no->pai->direita = filhoEsquerda;
    }
    filhoEsquerda->direita = no;
    no->pai = filhoEsquerda;
    return raiz;
}

void trocarCoresAtor(NodeAtor* no1, NodeAtor* no2) {
    bool corTemp = no1->isRed;
    no1->isRed = no2->isRed;
    no2->isRed = corTemp;
}

NodeAtor* corrigirInsercaoAtor(NodeAtor* raiz, NodeAtor* no) {
    while (no != raiz && no->pai->isRed) {
        if (no->pai == no->pai->pai->esquerda) {
            NodeAtor* tio = no->pai->pai->direita;
            if (tio != NULL && tio->isRed == 0) {
                no->pai->isRed = 1;
                tio->isRed = 1;
                no->pai->pai->isRed = 0;
                no = no->pai->pai;
            } else {
                if (no == no->pai->direita) {
                    no = no->pai;
                    raiz = rotacionarEsquerdaAtor(raiz, no);
                }
                no->pai->isRed = 1;
                no->pai->pai->isRed = 0;
                raiz = rotacionarDireitaAtor(raiz, no->pai->pai);
            }
        } else {
            NodeAtor* tio = no->pai->pai->esquerda;
            if (tio != NULL && tio->isRed == 0) {
                no->pai->isRed = 1;
                tio->isRed = 1;
                no->pai->pai->isRed = 0;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esquerda) {
                    no = no->pai;
                    raiz = rotacionarDireitaAtor(raiz, no);
                }
                no->pai->isRed = 1;
                no->pai->pai->isRed = 0;
                raiz = rotacionarEsquerdaAtor(raiz, no->pai->pai);
            }
        }
    }
    raiz->isRed = false;
    return raiz;
}


NodeAtor* inserirNoAtor(NodeAtor* raiz, int chave, const char* nomeAtor, const int* filmes, int numFilmes) {
    NodeAtor* novoNo = criarNoAtor(chave, nomeAtor);
    novoNo->Filmes = (int*)malloc(numFilmes * sizeof(int));
    memcpy(novoNo->Filmes, filmes, numFilmes * sizeof(int));
    novoNo->numFilmes = numFilmes;

    NodeAtor* pai = NULL;
    NodeAtor* atual = raiz;
    while (atual != NULL) {
        pai = atual;
        if (chave < atual->chave) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }

    novoNo->pai = pai;
    if (pai == NULL) {
        raiz = novoNo;
    } else if (chave < pai->chave) {
        pai->esquerda = novoNo;
    } else {
        pai->direita = novoNo;
    }

    raiz = corrigirInsercaoAtor(raiz, novoNo);
    return raiz;
}


NodeAtor* buscarNoAtor(NodeAtor* raiz, int chave) {
    NodeAtor* atual = raiz;
    while (atual != NULL && atual->chave != chave) {
        if (chave < atual->chave) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }
    return atual;
}

void imprimirNoAtor(NodeAtor* raiz, int chave) {
    NodeAtor* noEncontrado = buscarNoAtor(raiz,chave);
    printf("\n\nChave: %d Ator: %s  Filmes: ",noEncontrado->chave,noEncontrado->Ator);
    for (int i = 0; i < raiz->numFilmes; i++) {
            printf("%d  ", raiz->Filmes[i]);
        }
}

