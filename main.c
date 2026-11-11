#include <stdio.h>
#include <string.h>
#include "tabela.h"

#define M 13   /* número primo pequeno, para as colisões aparecerem */

/* lê uma linha do teclado e tira o '\n' do final */
void lerTexto(const char *pergunta, char *texto, int tamanho) {
    printf("%s", pergunta);
    if (fgets(texto, tamanho, stdin) == NULL) texto[0] = '\0';
    texto[strcspn(texto, "\n")] = '\0';
}

int main() {
    Tabela *agenda = criarTabela(M);
    char opcao[10], telefone[30], nome[50], email[50], obs[100];

    while (1) {
        printf("\n=== AGENDA DE CONTATOS ===\n");
        printf("1 - Inserir\n2 - Consultar\n3 - Atualizar\n4 - Remover\n");
        printf("5 - Listar\n6 - Exibir estrutura\n0 - Sair\n");
        lerTexto("Opcao: ", opcao, 10);

        if (opcao[0] == '0') break;

        if (opcao[0] == '1') {
            lerTexto("Telefone: ", telefone, 30);
            lerTexto("Nome: ", nome, 50);
            lerTexto("E-mail: ", email, 50);
            lerTexto("Observacoes: ", obs, 100);
            int r = inserir(agenda, telefone, nome, email, obs);
            if (r == 1) printf("Contato inserido.\n");
            else if (r == 0) printf("Esse telefone ja existe. Use Atualizar.\n");
            else printf("Telefone invalido.\n");
        }
        else if (opcao[0] == '2') {
            lerTexto("Telefone: ", telefone, 30);
            Contato *c = buscar(agenda, telefone);
            if (c == NULL) printf("Contato nao encontrado.\n");
            else printf("%s | %s | %s | %s\n", c->telefone, c->nome, c->email, c->obs);
        }
        else if (opcao[0] == '3') {
            lerTexto("Telefone: ", telefone, 30);
            lerTexto("Novo nome: ", nome, 50);
            lerTexto("Novo e-mail: ", email, 50);
            lerTexto("Novas observacoes: ", obs, 100);
            if (atualizar(agenda, telefone, nome, email, obs)) printf("Contato atualizado.\n");
            else printf("Contato nao encontrado.\n");
        }
        else if (opcao[0] == '4') {
            lerTexto("Telefone: ", telefone, 30);
            if (remover(agenda, telefone)) printf("Contato removido.\n");
            else printf("Contato nao encontrado.\n");
        }
        else if (opcao[0] == '5') listar(agenda);
        else if (opcao[0] == '6') exibirEstrutura(agenda);
        else printf("Opcao invalida.\n");
    }

    destruirTabela(agenda);
    return 0;
}
