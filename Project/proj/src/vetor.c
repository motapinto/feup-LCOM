#include "vetor.h"
#include <stdlib.h>
#include <stdio.h>

vetor* vetor_novo() {
	vetor *vec;

	/* aloca memoria para a estrutura vetor */
	vec = (vetor*)malloc(sizeof(vetor));
	if(vec == NULL)
		return NULL;
	vec->tamanho = 0;
	vec->capacidade = 0;
	vec->elementos = NULL;

	return vec;
}

void vetor_apaga(vetor* vec) {
	if(vec == NULL)
		return;
	for (int i=0; i<vec->tamanho; i++) {
		vetor_remove(vec, i);
	}
	free(vec->elementos);
	free(vec);
}


void vetor_insere(vetor* vec, int pos, int x, int y) { //vetor_insere(vec, -1, hres+14*j, y_pos+i*24);
	int i;

	if(vec == NULL || pos < -1 || pos > vec->tamanho)
		return ;

	/* aumenta elementos do vetor se capacidade nao for suficiente */
	if(vec->tamanho == vec->capacidade) {

		if(vec->capacidade == 0)
			vec->capacidade = 1;
		else
			vec->capacidade *= 2;

		vec->elementos = (v_elemento*)realloc(vec->elementos, vec->capacidade * sizeof(v_elemento));
	}

	/* se pos=-1 insere no fim do vetor */
	if(pos == -1)
		pos = vec->tamanho;
	/* copia todos os elementos a partir da posicao pos ate' ao fim do vetor para pos+1 */
	for(i = vec->tamanho-1; i >= pos; i--) {
		vec->elementos[i+1] = vec->elementos[i];
	}

	vec->elementos[pos].x = x ;
	vec->elementos[pos].y = y ;

	vec->tamanho++;
}

int vetor_remove(vetor* vec, int pos) {
	int i;

	if(vec == NULL || pos < 0 || pos >= vec->tamanho)
		return -1;

	/* copia todos os elementos a partir da posicao pos+1 ate' ao fim do vetor para pos */
	for(i=pos+1; i<vec->tamanho; i++) {
		vec->elementos[i-1] = vec->elementos[i];

	}

	vec->tamanho--;

	return 0;
}

void vetor_eliminate(vetor* vec)
{
	vetor_apaga(vec);
	vec = vetor_novo();
}
