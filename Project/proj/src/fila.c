
#include <stdlib.h>
#include "fila.h"

fila* fila_nova()
{
	fila *f = (fila *) malloc(sizeof(fila));
	if (f == NULL)
		return NULL;
	f->cabeca = f->cauda = NULL;

	return f;
}

void fila_apaga(fila *f)
{
	filaItem * cur, * next;

	if(f == NULL)
		return;

	if (f->cabeca != NULL)
	{
		cur = f->cabeca;
		while (cur->proximo != NULL)
		{
			free(cur->p);
			next = cur->proximo;
			free(cur);
			cur = next;
		}
	}

	free(f);
}

int	fila_vazia(fila *f)
{
	if(f == NULL)
		return -1;

	return f->cabeca == NULL;
}

int	fila_tamanho(fila *f)
{
	int count = 0;
	filaItem *cur;

	if(f == NULL)
		return -1;

	cur = f->cabeca;
	while(cur != NULL)
	{
		count++;
		cur = cur->proximo;
	}
	return count;
}

void * fila_front(fila *f)
{
	if (f == NULL || fila_vazia(f))
		return NULL;
	return f->cabeca->p;
}

int	fila_push(fila *f, void *p)
{
	filaItem *novo, *ult = f->cauda;

	if(f == NULL || p == NULL)
		return -1;

	novo = (filaItem *) malloc(sizeof(filaItem));
	if(novo == NULL)
		return -1;

	novo->p = p;
	novo->proximo = NULL;

	if (ult == NULL)
		f->cabeca = f->cauda = novo;
	else
	{
		f->cauda->proximo = novo;
		f->cauda = novo;
	}

	return 1;
}

void* fila_pop(fila *f)
{
	filaItem *cabeca;

	if (f == NULL || fila_vazia(f))
		return NULL;

	cabeca = f->cabeca;
	f->cabeca = cabeca->proximo;

	if (f->cabeca == NULL)
		f->cauda = NULL;

	return cabeca->p;
}
