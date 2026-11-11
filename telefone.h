#ifndef TELEFONE_H
#define TELEFONE_H

/* Normaliza o telefone e converte em chave numérica.
 * Ex.: "(84) 99812-3344" -> "5584998123344" -> 5584998123344
 * Retorna 1 se o telefone for válido e 0 se for inválido. */
int normalizarTelefone(const char *entrada, char *saida, unsigned long long *chave);

/* H1 - método da divisão: h(k) = k mod m */
int funcHash(unsigned long long chave, int m);

#endif
