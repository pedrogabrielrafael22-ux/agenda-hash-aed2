all:
	gcc -Wall -o agenda main.c tabela.c lista.c telefone.c
	gcc -Wall -o testes testes.c tabela.c lista.c telefone.c
	gcc -Wall -O2 -o experimento experimento.c tabela.c lista.c telefone.c

clean:
	rm -f agenda testes experimento
