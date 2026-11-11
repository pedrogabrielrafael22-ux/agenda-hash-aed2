#ifndef LISTA_H
#define LISTA_H

/* Valor guardado na agenda (A3) */
typedef struct {
    char telefone[16];   /* chave normalizada */
    char nome[50];
    char email[50];
    char obs[100];
} Contato;

/* E2.4 - nó da lista circular duplamente encadeada */
typedef struct No {
    unsigned long long chave;
    Contato contato;
    struct No *prox;
    struct No *ant;
} No;

typedef struct {
    No *cabeca;    /* NULL quando a lista está vazia */
    int tamanho;
} Lista;

void iniciarLista(Lista *l);
void inserirNoFim(Lista *l, No *novo);
No *buscarNaLista(Lista *l, unsigned long long chave);
void retirarDaLista(Lista *l, No *no);
void liberarLista(Lista *l);

#endif
