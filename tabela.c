#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela.h"
#include "telefone.h"

/* copia o texto sem passar do tamanho do campo */
static void copiar(char *destino, const char *origem, int tamanho) {
    snprintf(destino, tamanho, "%s", origem);
}

Tabela *criarTabela(int m) {
    Tabela *t = malloc(sizeof(Tabela));
    t->baldes = malloc(m * sizeof(Lista));
    for (int i = 0; i < m; i++) {
        iniciarLista(&t->baldes[i]);
    }
    t->m = m;
    t->quantidade = 0;
    t->ocupados = 0;
    t->colisoes = 0;
    return t;
}

void destruirTabela(Tabela *t) {
    for (int i = 0; i < t->m; i++) {
        liberarLista(&t->baldes[i]);
    }
    free(t->baldes);
    free(t);
}

int inserir(Tabela *t, const char *telefone, const char *nome, const char *email, const char *obs) {
    char normalizado[16];
    unsigned long long chave;

    if (!normalizarTelefone(telefone, normalizado, &chave)) return -1;

    int indice = funcHash(chave, t->m);
    Lista *balde = &t->baldes[indice];

    /* duplicata: não insere de novo */
    if (buscarNaLista(balde, chave) != NULL) return 0;

    No *novo = malloc(sizeof(No));
    novo->chave = chave;
    copiar(novo->contato.telefone, normalizado, 16);
    copiar(novo->contato.nome, nome, 50);
    copiar(novo->contato.email, email, 50);
    copiar(novo->contato.obs, obs, 100);

    /* colisão: o balde da nova chave já tinha outra chave */
    if (balde->tamanho > 0) t->colisoes++;
    else t->ocupados++;

    inserirNoFim(balde, novo);
    t->quantidade++;
    return 1;
}

/* acha o nó do telefone (usado por buscar, atualizar e remover) */
static No *localizar(Tabela *t, const char *telefone, Lista **balde) {
    char normalizado[16];
    unsigned long long chave;

    if (!normalizarTelefone(telefone, normalizado, &chave)) return NULL;

    *balde = &t->baldes[funcHash(chave, t->m)];
    return buscarNaLista(*balde, chave);
}

Contato *buscar(Tabela *t, const char *telefone) {
    Lista *balde;
    No *no = localizar(t, telefone, &balde);
    if (no == NULL) return NULL;
    return &no->contato;
}

int atualizar(Tabela *t, const char *telefone, const char *nome, const char *email, const char *obs) {
    Lista *balde;
    No *no = localizar(t, telefone, &balde);
    if (no == NULL) return 0;

    /* a chave (telefone) não muda */
    copiar(no->contato.nome, nome, 50);
    copiar(no->contato.email, email, 50);
    copiar(no->contato.obs, obs, 100);
    return 1;
}

int remover(Tabela *t, const char *telefone) {
    Lista *balde;
    No *no = localizar(t, telefone, &balde);
    if (no == NULL) return 0;

    retirarDaLista(balde, no);
    free(no);
    t->quantidade--;
    if (balde->tamanho == 0) t->ocupados--;
    return 1;
}

void exibirEstrutura(Tabela *t) {
    printf("\n--- Estrutura da tabela ---\n");
    for (int i = 0; i < t->m; i++) {
        Lista *balde = &t->baldes[i];
        printf("[%2d] ", i);
        if (balde->cabeca == NULL) {
            printf("vazio\n");
            continue;
        }
        No *aux = balde->cabeca;
        do {
            printf("%s <-> ", aux->contato.telefone);
            aux = aux->prox;
        } while (aux != balde->cabeca);
        printf("(volta ao inicio)\n");
    }
    printf("n = %d | m = %d | alfa = %.2f | colisoes = %d | baldes ocupados = %d\n",
           t->quantidade, t->m, (double)t->quantidade / t->m, t->colisoes, t->ocupados);
}

void listar(Tabela *t) {
    printf("\n--- Contatos ---\n");
    for (int i = 0; i < t->m; i++) {
        Lista *balde = &t->baldes[i];
        if (balde->cabeca == NULL) continue;
        No *aux = balde->cabeca;
        do {
            printf("%s | %s | %s | %s\n", aux->contato.telefone,
                   aux->contato.nome, aux->contato.email, aux->contato.obs);
            aux = aux->prox;
        } while (aux != balde->cabeca);
    }
    printf("Total: %d contato(s)\n", t->quantidade);
}
