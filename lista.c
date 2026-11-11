#include <stdlib.h>
#include "lista.h"

void iniciarLista(Lista *l) {
    l->cabeca = NULL;
    l->tamanho = 0;
}

void inserirNoFim(Lista *l, No *novo) {
    if (l->cabeca == NULL) {
        /* lista vazia: o nó aponta para ele mesmo */
        novo->prox = novo;
        novo->ant = novo;
        l->cabeca = novo;
    } else {
        /* o último é sempre o anterior da cabeça */
        No *ultimo = l->cabeca->ant;
        novo->prox = l->cabeca;
        novo->ant = ultimo;
        ultimo->prox = novo;
        l->cabeca->ant = novo;
    }
    l->tamanho++;
}

No *buscarNaLista(Lista *l, unsigned long long chave) {
    if (l->cabeca == NULL) return NULL;

    /* não existe NULL no fim: para quando volta à cabeça */
    No *aux = l->cabeca;
    do {
        if (aux->chave == chave) return aux;
        aux = aux->prox;
    } while (aux != l->cabeca);

    return NULL;
}

void retirarDaLista(Lista *l, No *no) {
    if (no->prox == no) {
        /* era o único nó */
        l->cabeca = NULL;
    } else {
        /* os vizinhos passam a apontar um para o outro */
        no->ant->prox = no->prox;
        no->prox->ant = no->ant;
        /* se era a cabeça, a cabeça anda uma casa */
        if (l->cabeca == no) l->cabeca = no->prox;
    }
    l->tamanho--;
}

void liberarLista(Lista *l) {
    while (l->cabeca != NULL) {
        No *no = l->cabeca;
        retirarDaLista(l, no);
        free(no);
    }
}
