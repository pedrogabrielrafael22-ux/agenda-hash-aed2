#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tabela.h"

#define M 1009          /* número primo */
#define REPETICOES 5

/* tempo atual em milissegundos */
double agora() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

int main() {
    int cenarios[3] = {100, 1000, 10000};

    printf("| Registros | Cap. | alfa | Colisoes | Ins. (ms) | Busca ok (ms) | Busca aus. (ms) |\n");
    printf("|---|---|---|---|---|---|---|\n");

    for (int c = 0; c < 3; c++) {
        int n = cenarios[c];
        char (*existentes)[16] = malloc(n * sizeof *existentes);
        char (*ausentes)[16] = malloc(n * sizeof *ausentes);
        double somaIns = 0, somaOk = 0, somaAus = 0, somaColisoes = 0;

        for (int rep = 0; rep < REPETICOES; rep++) {
            /* telefones aleatórios: 4 dígitos sorteados + 4 dígitos do índice i,
             * então nunca se repetem. Existentes começam com 849 e ausentes com 859 */
            srand(rep + 1);
            for (int i = 0; i < n; i++) {
                int sorteado = rand() % 10000;
                sprintf(existentes[i], "849%04d%04d", sorteado, i);
                sprintf(ausentes[i], "859%04d%04d", sorteado, i);
            }

            Tabela *t = criarTabela(M);
            double inicio;

            inicio = agora();
            for (int i = 0; i < n; i++) inserir(t, existentes[i], "Nome", "email", "");
            somaIns += agora() - inicio;

            inicio = agora();
            for (int i = 0; i < n; i++) buscar(t, existentes[i]);
            somaOk += agora() - inicio;

            inicio = agora();
            for (int i = 0; i < n; i++) buscar(t, ausentes[i]);
            somaAus += agora() - inicio;

            somaColisoes += t->colisoes;
            destruirTabela(t);
        }

        printf("| %d | %d | %.2f | %.1f | %.4f | %.4f | %.4f |\n",
               n, M, (double)n / M, somaColisoes / REPETICOES,
               somaIns / REPETICOES, somaOk / REPETICOES, somaAus / REPETICOES);

        free(existentes);
        free(ausentes);
    }
    printf("\nValores = media de %d repeticoes. Tempos = total das n operacoes.\n", REPETICOES);
    return 0;
}
