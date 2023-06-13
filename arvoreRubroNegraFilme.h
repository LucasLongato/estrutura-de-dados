#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Estrutura de um nó
typedef struct NodeFilme {
    int chave;
    char titulo[BUFSIZ]; //Coluna 3
    char genero[BUFSIZ]; //Coluna 8
    int ano;             //Coluna 0
    int isRed; // 0 = red | 1 = black
    struct NodeFilme* pai;
    struct NodeFilme* esquerda;
    struct NodeFilme* direita;
} NodeFilme;


NodeFilme* criarNoFilme(int chave, const char* titulo, const char* genero, int ano) {
    NodeFilme* novoNo = (NodeFilme*)malloc(sizeof(NodeFilme));
    novoNo->chave = chave;
    strcpy(novoNo->titulo, titulo);
    strcpy(novoNo->genero, genero);
    novoNo->ano = ano;
    novoNo->isRed = 0;
    novoNo->pai = NULL;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}


void liberarNoFilme(NodeFilme* no) {
    if (no != NULL) {
        free(no);
    }
}


void liberarArvoreFilme(NodeFilme* raiz) {
    if (raiz != NULL) {
        liberarArvoreFilme(raiz->esquerda);
        liberarArvoreFilme(raiz->direita);
        liberarNoFilme(raiz);
    }
}

void imprimirArvoreFilme(NodeFilme* raiz) {
    if (raiz != NULL) {
        printf("[");
        imprimirArvoreFilme(raiz->esquerda);
        printf(" Chave: %d, Titulo: %s, Genero: %s, Ano: %d", raiz->chave, raiz->titulo, raiz->genero, raiz->ano);
        imprimirArvoreFilme(raiz->direita);
        printf("]");
    }
}


NodeFilme* rotacionarEsquerdaFilme(NodeFilme* raiz, NodeFilme* no) {
    NodeFilme* filhoDireita = no->direita;
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


NodeFilme* rotacionarDireitaFilme(NodeFilme* raiz, NodeFilme* no) {
    NodeFilme* filhoEsquerda = no->esquerda;
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

void trocarCoresFilme(NodeFilme* no1, NodeFilme* no2) {
    bool corTemp = no1->isRed;
    no1->isRed = no2->isRed;
    no2->isRed = corTemp;
}

NodeFilme* corrigirInsercaoFilme(NodeFilme* raiz, NodeFilme* no) {
    while (no != raiz && no->pai->isRed) {
        if (no->pai == no->pai->pai->esquerda) {

            NodeFilme* tio = no->pai->pai->direita;
            if (tio != NULL && tio->isRed == 0) {
                no->pai->isRed = 1;
                tio->isRed = 1;
                no->pai->pai->isRed = 0;
                no = no->pai->pai;
            } else {
                if (no == no->pai->direita) {
                    no = no->pai;
                    raiz = rotacionarEsquerdaFilme(raiz, no);
                }
                no->pai->isRed = 1;
                no->pai->pai->isRed = 0;
                raiz = rotacionarDireitaFilme(raiz, no->pai->pai);
            }
        } else {
            NodeFilme* tio = no->pai->pai->esquerda;
            if (tio != NULL && tio->isRed == 0) {
                no->pai->isRed = 1;
                tio->isRed = 1;
                no->pai->pai->isRed = 0;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esquerda) {
                    no = no->pai;
                    raiz = rotacionarDireitaFilme(raiz, no);
                }
                no->pai->isRed = 1;
                no->pai->pai->isRed = 0;
                raiz = rotacionarEsquerdaFilme(raiz, no->pai->pai);
            }
        }
    }
    raiz->isRed = false;

    return raiz;
}


NodeFilme* inserirNoFilme(NodeFilme* raiz, int chave, const char* titulo, const char* genero, int ano) {
    NodeFilme* novoNo = criarNoFilme(chave, titulo, genero, ano);
    NodeFilme* pai = NULL;
    NodeFilme* atual = raiz;
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

    raiz = corrigirInsercaoFilme(raiz, novoNo);
    return raiz;
}


NodeFilme* buscarNoFilme(NodeFilme* raiz, int chave) {
    NodeFilme* atual = raiz;
    while (atual != NULL && atual->chave != chave) {
        if (chave < atual->chave) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }
    return atual;
}

void imprimirNoFilme(NodeFilme* raiz, int chave) {
    NodeFilme* noEncontrado = buscarNoFilme(raiz,chave);
    printf("\n\nChave: %d, Titulo: %s, Genero: %s, Ano: %d", raiz->chave, raiz->titulo, raiz->genero, raiz->ano);
}

