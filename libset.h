/** Tipo Abstrato de Dados "conjunto de inteiros" **/
/** implementado usando um vetor booleano **/
/** Carlos Maziero - DINF/UFPR, Set 2023 **/

/** Esta implementacao de conjunto usa um array de flags booleanos para representar **/
/** o conteudo do conjunto: se flag[x] == true, entao x esta no conjunto **/

#ifndef _SET_H
#define _SET_H

#include <stdbool.h>

/** representa um conjunto de inteiros **/
struct set_t {
  int tamanho;      /** numero maximo de valores para armazenar **/
  int num;          /** cardinalidade (numero atual de elementos) **/
  bool *flag;       /** aponta para um array de "tamanho" booleanos **/
};

/** Cria um conjunto vazio capaz de armazenar "tamanho" valores (de 0 a tamanho - 1) **/
/** Retorna: ponteiro para o novo conjunto ou NULL em caso de falha **/
struct set_t *set_criar(int max);

/** Destroi o conjunto, liberando sua memoria **/
/** Retorna: NULL **/
struct set_t *set_destruir(struct set_t *s);

/** Adiciona um item ao conjunto. **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_adicionar(struct set_t *s, int item);

/** Remove um item do conjunto. **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_remover(struct set_t *s, int item);

/** Verifica se o item pertence (esta no) ao conjunto. **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_pertence(struct set_t *s, int item);

/** Verifica se o conjunto esta vazio **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_vazio(struct set_t *s);

/** Informa a cardinalidade do conjunto (numero de itens ou |S|) **/
/** Retorna: n >= 0 em caso de sucesso ou -1 em caso de falha **/
int set_cardinalidade(struct set_t *s);

/** Verifica se s1 contem s2 **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_contem(struct set_t *s1, struct set_t *s2);

/** Verifica se s1 == s2 **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_igual(struct set_t *s1, struct set_t *s2);

/** Coloca em s3 a uniao dos conjuntos s1 e s2; s3 conteudo anterior **/
/** e limpo, mas seu intervalo (0, max) nao e modificado **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_uniao(struct set_t *s1, struct set_t *s2, struct set_t *s3);

/** Coloca em s3 a intersecao dos conjuntos s1 e s2; s3 conteudo anterior **/
/** e limpo, mas seu intervalo (0, max) nao e modificado **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_intersecao(struct set_t *s1, struct set_t *s2, struct set_t *s3);

/** Coloca em s3 a diferenca s1 - s2; s3 conteudo anterior e limpo, **/
/** mas seu intervalo (0, max) nao e modificado **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_diferenca(struct set_t *s1, struct set_t *s2, struct set_t *s3);

/** Coloca em s2 uma copia do conjunto s1; s2 conteudo anterior e limpo, **/
/** mas seu intervalo (0, max) nao e modificado **/
/** Retorna: 1 em caso de sucesso ou 0 em caso de falha **/
int set_copiar(struct set_t *s1, struct set_t *s2);

/** Imprime o conteudo de um conjunto usando este modelo: **/
/** [ %d %d %d ] \n **/
/** (Nao pertence a definicao do TAD, mas e util para testes) **/
void set_imprimir(struct set_t *s);

#endif

