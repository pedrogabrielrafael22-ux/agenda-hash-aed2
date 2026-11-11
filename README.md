# Agenda de Contatos com Tabela Hash — Configuração 255

**Disciplina:** Algoritmos e Estrutura de Dados II — Laboratório (UFERSA)
**Professor:** Kennedy Lopes
**Grupo:** Pedro Gabriel Rafael de Freitas · Rubens Guilherme Lopes do Nascimento · Luan Kauê Rodrigues da Silva

## Configuração

| Código | Significado |
|---|---|
| **E2.4** | Cada balde da tabela é uma lista circular duplamente encadeada |
| **H1** | Método da divisão: `h(k) = k mod m` |
| **C1** | Encadeamento separado: chaves que colidem ficam na lista do mesmo balde |
| **A3** | Agenda de contatos: chave = telefone normalizado; valor = nome, e-mail e observações |

Nenhuma estrutura pronta (dict, HashMap etc.) foi usada.

## Arquivos

| Arquivo | Conteúdo |
|---|---|
| `telefone.c` | Normalização do telefone e função hash (H1) |
| `lista.c` | Lista circular duplamente encadeada (E2.4) |
| `tabela.c` | Tabela hash: inserir, buscar, atualizar, remover, exibirEstrutura, listar (C1) |
| `main.c` | Agenda de contatos com menu (A3) |
| `testes.c` | Testes de correção obrigatórios |
| `experimento.c` | Análise experimental |

## Como compilar e executar

```bash
make
./agenda
./testes
./experimento
```

Sem `make` (Windows):

```bash
gcc -o agenda main.c tabela.c lista.c telefone.c
gcc -o testes testes.c tabela.c lista.c telefone.c
gcc -O2 -o experimento experimento.c tabela.c lista.c telefone.c
```

## Projeto

### Desenho da tabela

```
baldes[0] -> vazio
baldes[1] -> [A] <-> [B] <-> [C] -> volta para [A]
baldes[2] -> [D] -> volta para [D]
...
baldes[m-1] -> vazio
```

Cada nó guarda a chave, o contato e os ponteiros `prox` e `ant`. Não existe `NULL` no fim da lista: o último nó aponta para a cabeça, e o último é sempre `cabeca->ant`.

### Conversão da chave

1. Fica só com os dígitos (tira espaços, hífens, parênteses e `+`).
2. Tira os zeros à esquerda.
3. Com 10 ou 11 dígitos (DDD + número), coloca `55` na frente.
4. Com 12 ou 13 dígitos, aceita só se começar com `55`.
5. O DDD não pode ter o dígito 0.
6. Qualquer outro caso é telefone inválido.

Assim, `(84) 99812-3344`, `84998123344` e `+55 84 99812-3344` viram a mesma chave: `5584998123344`. Ela é convertida em número com `strtoull` (`unsigned long long`, 64 bits).

### Cálculo do índice

`indice = chave % m`. Exemplo: `5584999991234 % 1009 = 165`.

### Políticas

| Situação | Comportamento |
|---|---|
| Colisão | A chave nova entra no fim da lista do balde. Conta como colisão quando o balde já tinha outra chave. |
| Duplicata | Não insere; `inserir` retorna 0. Para mudar os dados, usa-se `atualizar`. |
| Busca sem resultado | `buscar` retorna `NULL`; `atualizar` e `remover` retornam 0. |
| Telefone inválido | `inserir` retorna -1. |

### Capacidade e parâmetros

- Capacidade fixa (E2.4 não redimensiona): `m = 13` na agenda, para as colisões aparecerem, e `m = 1009` no experimento.
- `m` é primo para que todos os dígitos do telefone influenciem o resto da divisão.

## Testes de correção

| Teste | Entrada | Esperado | Obtido | Situação |
|---|---|---|---|---|
| 1. Inserção e busca | inserir `(84) 99999-1234`; buscar `84999991234` | Ana | Ana | Aprovado |
| 2. Chave inexistente | buscar `(11) 98888-7777` | não encontrado | não encontrado | Aprovado |
| 3. Duplicata | inserir `+55 84 99999-1234` | retorno 0, n = 1 | retorno 0, n = 1 | Aprovado |
| 4. Colisão | A, B e C com índice 4 (m = 7) | balde 4 com 3, colisões = 2 | balde 4 com 3, colisões = 2 | Aprovado |
| 5. Remoção em colisão | remover B; buscar A, B, C | A e C encontrados, B não | A e C encontrados, B não | Aprovado |
| 6. Atualização | atualizar e-mail de C | e-mail novo, mesma chave | e-mail novo, mesma chave | Aprovado |
| 7a. Tabela vazia | buscar e remover | não encontrado, 0 | não encontrado, 0 | Aprovado |
| 7b. Telefone inválido | `9999-1234` e `abc` | -1 e -1 | -1 e -1 | Aprovado |
| 7c. Remover único | inserir e remover | n = 0, ocupados = 0 | n = 0, ocupados = 0 | Aprovado |
| 8. Redimensionamento | — | Não se aplica (só E1.2) | — | — |

No teste 4, as chaves `5584999990000`, `5584999990007` e `5584999990014` diferem por múltiplos de 7, então todas têm resto 4.

## Análise experimental

100, 1.000 e 10.000 contatos, m = 1009, média de 5 repetições. Os telefones ausentes nunca foram inseridos.

| Registros | Cap. | α | Colisões | Ins. (ms) | Busca ok (ms) | Busca aus. (ms) |
|---|---|---|---|---|---|---|
| 100 | 1009 | 0,10 | 4,2 | 0,0333 | 0,0055 | 0,0055 |
| 1.000 | 1009 | 0,99 | 364,0 | 0,2190 | 0,0579 | 0,0611 |
| 10.000 | 1009 | 9,91 | 8.991,0 | 3,1422 | 1,3257 | 2,3783 |

Os tempos variam conforme o computador.

## Discussão

- Quanto maior o fator de carga α = n/m, mais colisões e maiores as listas. Com 10.000 contatos em 1009 baldes, cada lista tem em média quase 10 nós.
- Por isso o tempo de busca cresceu bem mais que o número de contatos entre 1.000 e 10.000.
- A busca de chave ausente foi a mais lenta com α alto, porque percorre a lista inteira do balde; a busca que acha a chave para no meio, em média.
- Mesmo com α < 1 há colisões (364 com 1.000 contatos), mas o encadeamento as resolve sem perder nenhum contato.
- Como a capacidade é fixa, o ideal é escolher m próximo do número de contatos esperado.
