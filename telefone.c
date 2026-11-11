#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "telefone.h"

int normalizarTelefone(const char *entrada, char *saida, unsigned long long *chave) {
    char digitos[20];
    int n = 0;

    /* 1. fica só com os dígitos */
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (isdigit((unsigned char)entrada[i])) {
            if (n >= 19) return 0;
            digitos[n++] = entrada[i];
        }
    }
    digitos[n] = '\0';

    /* 2. tira os zeros à esquerda (ex.: 084...) */
    char *p = digitos;
    while (*p == '0') p++;
    int tam = strlen(p);

    /* 3. convenção: 55 + DDD + número */
    if (tam == 10 || tam == 11) {
        strcpy(saida, "55");
        strcat(saida, p);
    } else if ((tam == 12 || tam == 13) && p[0] == '5' && p[1] == '5') {
        strcpy(saida, p);
    } else {
        return 0;
    }

    /* 4. o DDD não tem o dígito zero */
    if (saida[2] == '0' || saida[3] == '0') return 0;

    /* 5. converte a string em número de 64 bits */
    *chave = strtoull(saida, NULL, 10);
    return 1;
}

int funcHash(unsigned long long chave, int m) {
    return (int)(chave % m);
}
