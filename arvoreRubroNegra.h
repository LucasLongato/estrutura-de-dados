#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Estrutura de um nó
typedef struct Node {
    int chave;
    char Ator[BUFSIZ];
    int isRed; // 0 = red | 1 = black
    struct Node* pai;
    struct Node* esquerda;
    struct Node* direita;
    int numFilmes;
    int* Filmes;
} Node;


Node* criarNo(int chave, const char* nomeAtor) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
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


void liberarNo(Node* no) {
    if (no != NULL) {
        free(no->Filmes);
        free(no);
    }
}


void liberarArvore(Node* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        liberarNo(raiz);
    }
}

void imprimirArvore(Node* raiz) {
    if (raiz != NULL) {
        printf("[");
        imprimirArvore(raiz->esquerda);
        printf(" Chave: %d, Ator: %s ", raiz->chave, raiz->Ator);
        for (int i = 0; i < raiz->numFilmes; i++) {
            printf("%d  ", raiz->Filmes[i]);
        }
        imprimirArvore(raiz->direita);
        printf("]");
    }
}


Node* rotacionarEsquerda(Node* raiz, Node* no) {
    Node* filhoDireita = no->direita;
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


Node* rotacionarDireita(Node* raiz, Node* no) {
    Node* filhoEsquerda = no->esquerda;
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

void trocarCores(Node* no1, Node* no2) {
    bool corTemp = no1->isRed;
    no1->isRed = no2->isRed;
    no2->isRed = corTemp;
}

Node* corrigirInsercao(Node* raiz, Node* no) {
    while (no != raiz && no->pai->isRed) {
        if (no->pai == no->pai->pai->esquerda) {
            Node* tio = no->pai->pai->direita;
            if (tio != NULL && tio->isRed == 0) {
                no->pai->isRed = 1;
                tio->isRed = 1;
                no->pai->pai->isRed = 0;
                no = no->pai->pai;
            } else {
                if (no == no->pai->direita) {
                    no = no->pai;
                    raiz = rotacionarEsquerda(raiz, no);
                }
                no->pai->isRed = 1;
                no->pai->pai->isRed = 0;
                raiz = rotacionarDireita(raiz, no->pai->pai);
            }
        } else {
            Node* tio = no->pai->pai->esquerda;
            if (tio != NULL && tio->isRed == 0) {
                no->pai->isRed = 1;
                tio->isRed = 1;
                no->pai->pai->isRed = 0;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esquerda) {
                    no = no->pai;
                    raiz = rotacionarDireita(raiz, no);
                }
                no->pai->isRed = 1;
                no->pai->pai->isRed = 0;
                raiz = rotacionarEsquerda(raiz, no->pai->pai);
            }
        }
    }
    raiz->isRed = false;
    return raiz;
}


Node* inserirNoAtor(Node* raiz, int chave, const char* nomeAtor, const int* filmes, int numFilmes) {
    Node* novoNo = criarNo(chave, nomeAtor);
    novoNo->Filmes = (int*)malloc(numFilmes * sizeof(int));
    memcpy(novoNo->Filmes, filmes, numFilmes * sizeof(int));
    novoNo->numFilmes = numFilmes;

    Node* pai = NULL;
    Node* atual = raiz;
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

    raiz = corrigirInsercao(raiz, novoNo);
    return raiz;
}


Node* buscarNo(Node* raiz, int chave) {
    Node* atual = raiz;
    while (atual != NULL && atual->chave != chave) {
        if (chave < atual->chave) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }
    return atual;
}

void imprimirNo(Node* raiz, int chave) {
    Node* noEncontrado = buscarNo(raiz,chave);
    printf("\n\nChave: %d Ator: %s  Filmes: ",noEncontrado->chave,noEncontrado->Ator);
    for (int i = 0; i < raiz->numFilmes; i++) {
            printf("%d  ", raiz->Filmes[i]);
        }

}

