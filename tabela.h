#ifndef TABELA_H
#define TABELA_H

#include "lista.h"

/* Configuração 255: E2.4 (lista circular dupla), H1 (divisão),
 * C1 (encadeamento separado), A3 (agenda de contatos) */
typedef struct {
    Lista *baldes;    /* vetor com m listas */
    int m;            /* número de baldes */
    int quantidade;   /* n: contatos guardados */
    int ocupados;     /* baldes com pelo menos um contato */
    int colisoes;
} Tabela;

Tabela *criarTabela(int m);
void destruirTabela(Tabela *t);

/* inserir e atualizar: 1 = ok, 0 = duplicado / não encontrado, -1 = telefone inválido */
int inserir(Tabela *t, const char *telefone, const char *nome, const char *email, const char *obs);
int atualizar(Tabela *t, const char *telefone, const char *nome, const char *email, const char *obs);

/* buscar: devolve o contato ou NULL se não existir */
Contato *buscar(Tabela *t, const char *telefone);

/* remover: 1 = removido, 0 = não encontrado */
int remover(Tabela *t, const char *telefone);

void exibirEstrutura(Tabela *t);
void listar(Tabela *t);

#endif
