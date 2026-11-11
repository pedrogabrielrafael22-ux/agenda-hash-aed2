#include <stdio.h>
#include <string.h>
#include "tabela.h"

int total = 0, aprovados = 0;

void mostrar(const char *caso, const char *entrada, const char *esperado,
             const char *obtido, int passou) {
    total++;
    if (passou) aprovados++;
    printf("\n%s\n", caso);
    printf("  Entrada:  %s\n", entrada);
    printf("  Esperado: %s\n", esperado);
    printf("  Obtido:   %s\n", obtido);
    printf("  Situacao: %s\n", passou ? "APROVADO" : "REPROVADO");
}

int main() {
    char obtido[200];
    Contato *c;
    int r;

    /* ---------- Testes 1 a 3 (m = 13) ---------- */
    Tabela *t = criarTabela(13);

    /* 1. Inserção e busca de chave existente */
    inserir(t, "(84) 99999-1234", "Ana", "ana@email.com", "");
    c = buscar(t, "84999991234");   /* mesmo telefone, outro formato */
    sprintf(obtido, "%s", c ? c->nome : "nao encontrado");
    mostrar("TESTE 1 - Insercao e busca de chave existente",
            "inserir (84) 99999-1234 Ana; buscar 84999991234",
            "Ana", obtido, c != NULL && strcmp(c->nome, "Ana") == 0);

    /* 2. Busca de chave inexistente */
    c = buscar(t, "(11) 98888-7777");
    sprintf(obtido, "%s", c ? c->nome : "nao encontrado");
    mostrar("TESTE 2 - Busca de chave inexistente",
            "buscar (11) 98888-7777",
            "nao encontrado", obtido, c == NULL);

    /* 3. Tentativa de inserção duplicada */
    r = inserir(t, "+55 84 99999-1234", "Outra", "outra@email.com", "");
    c = buscar(t, "84999991234");
    sprintf(obtido, "retorno %d, n = %d, nome = %s", r, t->quantidade, c->nome);
    mostrar("TESTE 3 - Insercao duplicada",
            "inserir +55 84 99999-1234 Outra",
            "retorno 0, n = 1, nome = Ana", obtido,
            r == 0 && t->quantidade == 1 && strcmp(c->nome, "Ana") == 0);
    destruirTabela(t);

    /* ---------- Testes 4 a 6 (m = 7) ---------- */
    /* As três chaves diferem por múltiplos de 7, então têm o mesmo resto:
     * 5584999990000 mod 7 = 4
     * 5584999990007 mod 7 = 4
     * 5584999990014 mod 7 = 4 */
    t = criarTabela(7);
    inserir(t, "5584999990000", "A", "a@email.com", "");
    inserir(t, "5584999990007", "B", "b@email.com", "");
    inserir(t, "5584999990014", "C", "c@email.com", "");

    /* 4. Duas ou mais chaves com colisão demonstrada */
    exibirEstrutura(t);
    sprintf(obtido, "balde 4 com %d contatos, colisoes = %d",
            t->baldes[4].tamanho, t->colisoes);
    mostrar("TESTE 4 - Chaves com colisao",
            "inserir A, B e C (todas com indice 4)",
            "balde 4 com 3 contatos, colisoes = 2", obtido,
            t->baldes[4].tamanho == 3 && t->colisoes == 2);

    /* 5. Remoção em colisão e busca das chaves restantes */
    remover(t, "5584999990007");   /* B, o nó do meio */
    exibirEstrutura(t);
    int achouA = buscar(t, "5584999990000") != NULL;
    int achouB = buscar(t, "5584999990007") != NULL;
    int achouC = buscar(t, "5584999990014") != NULL;
    sprintf(obtido, "A %s, B %s, C %s",
            achouA ? "encontrado" : "nao encontrado",
            achouB ? "encontrado" : "nao encontrado",
            achouC ? "encontrado" : "nao encontrado");
    mostrar("TESTE 5 - Remocao em colisao",
            "remover B; buscar A, B e C",
            "A encontrado, B nao encontrado, C encontrado", obtido,
            achouA && !achouB && achouC);

    /* 6. Atualização sem alterar a chave */
    r = atualizar(t, "5584999990014", "C", "novo@email.com", "e-mail novo");
    c = buscar(t, "5584999990014");
    sprintf(obtido, "email = %s, chave = %s, n = %d, colisoes = %d",
            c->email, c->telefone, t->quantidade, t->colisoes);
    mostrar("TESTE 6 - Atualizacao sem alterar a chave",
            "atualizar C com email novo@email.com",
            "email = novo@email.com, chave = 5584999990014, n = 2, colisoes = 2", obtido,
            r == 1 && strcmp(c->email, "novo@email.com") == 0 &&
            strcmp(c->telefone, "5584999990014") == 0 &&
            t->quantidade == 2 && t->colisoes == 2);
    destruirTabela(t);

    /* ---------- Teste 7 (m = 7) ---------- */
    t = criarTabela(7);

    /* 7a. Estrutura vazia */
    c = buscar(t, "(84) 99999-0000");
    r = remover(t, "(84) 99999-0000");
    sprintf(obtido, "buscar: %s, remover: %d, n = %d",
            c ? "encontrado" : "nao encontrado", r, t->quantidade);
    mostrar("TESTE 7a - Tabela vazia",
            "buscar e remover em tabela vazia",
            "buscar: nao encontrado, remover: 0, n = 0", obtido,
            c == NULL && r == 0 && t->quantidade == 0);

    /* 7b. Telefone inválido */
    int r1 = inserir(t, "9999-1234", "X", "", "");      /* sem DDD */
    int r2 = inserir(t, "abc", "X", "", "");
    sprintf(obtido, "retornos %d e %d, n = %d", r1, r2, t->quantidade);
    mostrar("TESTE 7b - Telefone invalido",
            "inserir 9999-1234 (sem DDD) e abc",
            "retornos -1 e -1, n = 0", obtido,
            r1 == -1 && r2 == -1 && t->quantidade == 0);

    /* 7c. Remover o único contato de um balde */
    inserir(t, "(84) 99999-0000", "Unico", "", "");
    r = remover(t, "(84) 99999-0000");
    sprintf(obtido, "remover: %d, n = %d, ocupados = %d", r, t->quantidade, t->ocupados);
    mostrar("TESTE 7c - Remover o unico contato",
            "inserir e depois remover (84) 99999-0000",
            "remover: 1, n = 0, ocupados = 0", obtido,
            r == 1 && t->quantidade == 0 && t->ocupados == 0);
    destruirTabela(t);

    /* 8. Redimensionamento: só se aplica a E1.2 */
    printf("\nTESTE 8 - Redimensionamento: nao se aplica (E2.4 tem m fixo).\n");

    printf("\nRESULTADO: %d de %d testes aprovados\n", aprovados, total);
    return 0;
}
