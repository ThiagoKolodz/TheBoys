/** TAD lista de numeros inteiros - interface **/
/** Carlos Maziero - DINF/UFPR, Set 2023 **/
#ifndef _LISTA_H
#define _LISTA_H

/** estrutura de um nodo da lista **/
typedef struct lnode_t {
  int val ;            /** valor armazenado **/
  struct lnode_t *prev ;  /** nodo anterior **/
  struct lnode_t *next ;  /** nodo seguinte **/
} lnode_t;

/** estrutura de uma lista **/
typedef struct lista_t {
  lnode_t *head ;    /** primeiro elemento **/
  lnode_t *tail ;    /** ultimo elemento **/
  int size ;         /** tamanho da lista **/
} lista_t;

/** Cria uma lista vazia. **/
/** Retorno: ponteiro p/ a lista ou NULL se falhar. **/
lista_t* lista_cria ();

/** Remove todos os elementos da lista, libera espaco. **/
/** Retorno: NULL. **/
lista_t* lista_destroi (lista_t* lst);

/** Insere o elemento na lista na posicao indicada; a posicao 0 indica o **/
/** inicio da lista. Se a posicao for alem do fim da lista, insere no fim. **/
/** O fim da lista tambem pode ser indicado pela posicao -1. **/
/** Retorno: numero de elementos na lista apos a operacao ou -1 se falhar. **/
int lista_insere (lista_t* lst, int elem);

/** Retira o elemento da lista, na posicao indicada. **/
/** Retorno: numero de elementos na lista apos a operacao ou -1 se falhar. **/
int lista_retira (lista_t *lst, int *elemento);

/** Informa o valor do elemento na posicao indicada, sem retir -lo. **/
/** Retorno: numero de elementos na lista ou -1 se falhar. **/
int lista_consulta (lista_t* lst, int* elem, int pos);

/** Informa a posicao da 1a ocorrencia do elemento indicado. **/
/** Retorno: posicao do elemento ou -1 se nao encontrar ou falhar. **/
int lista_procura (lista_t* lst, int elem);

/** Informa se a lista esta vazia. **/
/** Retorno: 1 se a lista esta vazia ou 0 caso contrario. **/
int lista_vazia (lista_t* lst);

/** Informa o tamanho da lista (o numero de elementos presentes nela). **/
/** Retorno: numero de elementos na lista ou -1 se falhar. **/
int lista_tamanho (lista_t* lst);

/** Imprime o conteudo da lista, do inicio ao fim, neste formato: **/
/** nome: [ valor valor valor valor ... ] (nnn elementos) **/
/** Esta funcao nao faz parte do TAD, mas e util para testes. **/
void lista_imprime (lista_t* lst);

#endif

