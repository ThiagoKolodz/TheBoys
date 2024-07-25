/** TAD lista de numeros inteiros - implementacao **/
/** Autor: Thiago Fortes Kolodziejski **/

/** Implementacao com lista encadeada dupla nao-circular **/

/** includes **/
#include <stdio.h>
#include <stdlib.h>
#include "liblista.h"

/** Eventuais funcoes auxiliares (usadas somente neste arquivo) podem vir aqui. **/

/** Cria uma lista vazia. **/
/** Retorno: ponteiro p/ a lista ou NULL se falhar. **/
lista_t * lista_cria() {
    
    lista_t *lst = (lista_t*)malloc(sizeof(lista_t));
    if (lst) {
        
        lst->head = NULL;
        lst->tail = NULL;
        lst->size = 0;
        
    }
    
    return lst;
    
}

/** Remove todos os elementos da lista, libera espaco. **/
/** Retorno: NULL. **/
lista_t* lista_destroi(lista_t* lst) {
    if (lst) {
        lnode_t* current = lst->head;
        while (current) {
            lnode_t* next = current->next;
            free(current);
            current = next;
        }
        free(lst);
    }
    return NULL;
}

/** Informa se a lista esta vazia. **/
/** Retorno: 1 se a lista esta vazia ou 0 caso contrario. **/
int lista_vazia(lista_t* lst) {
    
    return lst ? (lst->size == 0) : 1;
    
}

/* Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario. 
 */
int lista_insere(lista_t* lst, int elem) {
    lnode_t *no = malloc(sizeof(lnode_t));
    if(!no)
        return 0;

    no->val = elem;
    no->next = NULL;
    if(lista_vazia(lst)){
        lst->head = no;
        lst->tail = no;
    }else{
        (lst->tail)->next = no;
        lst->tail = no;
    }
    lst->size++;

    return 1;
}

/** Retira o elemento da lista, na posicao indicada. **/
/** Retorno: numero de elementos na lista apos a operacao ou -1 se falhar. **/
/* Remove o elemento do inicio da fila (politica FIFO) e o retorna
 * no parametro *elemento. Retorna 1 se a operacao foi bem sucedida
 * e 0 caso contrario. */
int lista_retira(lista_t *lst, int *elemento){
    lnode_t *aux;
    if(lista_vazia(lst))
        return 0;
    
    aux = lst->head;
    lst->head = aux->next;
    *elemento = aux->val;
    lst->size--;
    free(aux);
    
    return 1;
}

/** Informa o valor do elemento na posicao indicada, sem retira-lo. **/
/** Retorno: numero de elementos na lista ou -1 se falhar. **/
int lista_consulta(lista_t* lst, int* elem, int pos) {
    if (!lst || pos < 0 || pos >= lst->size) return -1;
    
    lnode_t* current = lst->head;
    for (int i = 0; i < pos; i++) {
        if (!current) return -1;
        current = current->next;
    }
    
    if (elem) *elem = current->val;
    return lst->size;
}

/** Informa a posicao da 1a ocorrencia do elemento indicado. **/
/** Retorno: posicao do elemento ou -1 se nao encontrar ou falhar. **/
int lista_procura(lista_t* lst, int elem) {
    if (!lst) return -1;
    
    lnode_t* current = lst->head;
    int pos = 0;
    
    while (current) {
        if (current->val == elem) return pos;
        current = current->next;
        pos++;
    }
    
    return -1;
}

/** Informa o tamanho da lista (o numero de elementos presentes nela). **/
/** Retorno: numero de elementos na lista ou -1 se falhar. **/
int lista_tamanho(lista_t* lst) {
    
    return lst ? lst->size : -1;
    
}

/** Imprime o conteudo da lista, do inicio ao fim, neste formato: **/
/** nome: [ valor valor valor valor ... ] (nnn elementos) **/
/** Esta funcao nao faz parte do TAD, mas e util para testes. **/
void lista_imprime(lista_t* lst) {
    if (!lst) return;
    
    printf("[");
    lnode_t* current = lst->head;
    
    while (current) {
        printf(" %d", current->val);
        current = current->next;
    }
    
    printf(" ] \n");
}

