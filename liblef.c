#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"

LEF *criaLEF() {
    LEF *novaLEF = (LEF *)malloc(sizeof(LEF));
    if (novaLEF != NULL) {
        novaLEF->primeiro = NULL;
    }
    return novaLEF;
}

LEF *destroiLEF(LEF *lef) {
    if (lef != NULL) {
        NodoLEF *atual = lef->primeiro;
        while (atual != NULL) {
            NodoLEF *proximo = atual->prox;
            free(atual->evento);
            free(atual);
            atual = proximo;
        }
        free(lef);
    }
    return NULL;
}

int adicionaInicioLEF(LEF *lef, const Evento_t *evento) {
    if (lef == NULL || evento == NULL) {
        return 0; /* Falha */
    }

    NodoLEF *novoNodo = (NodoLEF *)malloc(sizeof(NodoLEF));
    if (novoNodo == NULL) {
        return 0; /* Falha */
    }

    // Cria uma c¢pia do evento
    novoNodo->evento = (Evento_t *)malloc(sizeof(Evento_t));
    if (novoNodo->evento == NULL) {
        free(novoNodo);
        return 0; /* Falha */
    }
    *(novoNodo->evento) = *evento;

    /* Adiciona o novo nodo no in¡cio da lista */
    novoNodo->prox = lef->primeiro;
    lef->primeiro = novoNodo;

    return 1; /* Sucesso */
}

int adicionaOrdemLEF(LEF *lef, const Evento_t *evento) {
    if (lef == NULL || evento == NULL) {
        return 0; /* Falha */
    }

    NodoLEF *novoNodo = (NodoLEF *)malloc(sizeof(NodoLEF));
    if (novoNodo == NULL) {
        return 0; /* Falha */
    }

    // Cria uma c¢pia do evento
    novoNodo->evento = (Evento_t *)malloc(sizeof(Evento_t));
    if (novoNodo->evento == NULL) {
        free(novoNodo);
        return 0; /* Falha */
    }
    *(novoNodo->evento) = *evento;

    /* Adiciona o novo nodo ordenadamente */
    NodoLEF *anterior = NULL;
    NodoLEF *atual = lef->primeiro;
    while (atual != NULL && atual->evento->tempo < evento->tempo) {
        anterior = atual;
        atual = atual->prox;
    }

    if (anterior == NULL) {
        /* Inser‡Æo no in¡cio da lista */
        novoNodo->prox = lef->primeiro;
        lef->primeiro = novoNodo;
    } else {
        /* Inser‡Æo no meio ou no final da lista */
        novoNodo->prox = atual;
        anterior->prox = novoNodo;
    }

    return 1; /* Sucesso */
}

Evento_t *obtemPrimeiroLEF(LEF *lef) {
    if (lef == NULL || lef->primeiro == NULL) {
        return NULL; // Lista vazia
    }

    /* Remove o primeiro nodo da lista e retorna o evento associado */
    NodoLEF *primeiroNodo = lef->primeiro;
    Evento_t *evento = primeiroNodo->evento;
    lef->primeiro = primeiroNodo->prox;
    free(primeiroNodo);

    return evento;
}

