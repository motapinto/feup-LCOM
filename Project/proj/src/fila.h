#ifndef FILA_H
#define FILA_H

/** @defgroup fila fila
 * @{
 *
 */

struct fitem
{
	void *p;
	struct fitem *proximo;
};

typedef struct fitem filaItem;

typedef struct
{
	filaItem *cabeca;
	filaItem *cauda;
} fila;

/**
 *  \brief cria uma nova fila vazia
 *  \return apontador para a fila criada
 *  \remark se a fila nao for criada, retorna NULL
 */
fila* fila_nova();

/**
 *  \brief esvazia a fila e liberta-a da memoria
 *  \param f_ref endereco do apontador para a fila em causa
 *	\remark o apontador para a fila em causa deve ficar a apontar para NULL
 */
void fila_apaga(fila *f);

/**
 *  \brief testa se a fila esta' vazia
 *  \param f apontador para a fila em causa
 *	\return 1, se estiver vazia. 0, caso contrario
 */
int	fila_vazia(fila *f);

/**
 *  \brief retorna o tamanho actual da fila
 *  \param f apontador para a fila em causa
 *  \return numero de strings actualmente na fila
 */
int fila_tamanho(fila *f);

/**
 *  \brief devolve string 'a cabeca da fila, sem a retirar
 *  \param f apontador para a fila em causa
 *  \return string 'a cabeca da fila
 *  \remark se a fila estiver vazia, retorna NULL
 */
void* fila_front(fila *f);

/**
 *  \brief insere string na cauda da fila
 *  \param f apontador para a fila em causa
 *  \param string string a inserir
 *  \return se inserir com sucesso, retorna 1
 *  \return noutras situacoes, retorna -1
 */
int fila_push(fila *f, void* string);

/**
 *  \brief retira e retorna a string 'a cabeca da fila
 *  \param f apontador para a fila em causa
 *  \remark se a fila estiver vazia, retorna NULL
 */
void* fila_pop(fila *f);

#endif
