#ifndef LEF_H
#define LEF_H

/** Definicao da estrutura evento_t */
typedef struct Evento_t {
    int tempo;   /** Tempo logico do evento */
    int tipo;    /** Inteiro para diferenciar os diferentes tipos de evento */
    int dado1;  /* um inteiro para guardar dados do evento */
    int dado2;  /* um inteiro para guardar dados do evento */
} Evento_t;

/** Definicao da estrutura nodo_lef_t */
typedef struct NodoLEF {
    Evento_t *evento;          /** Ponteiro para um evento_t que contem os dados do evento */
    struct NodoLEF *prox;    /** Ponteiro para o proximo nodo na Lista de Eventos Futuros */
} NodoLEF;

/** Definicao da estrutura lef_t */
typedef struct {
    NodoLEF *primeiro;  /** Ponteiro para o primeiro nodo na lista */
} LEF;

/** Funcoes oferecidas pelo TAD */

/** Cria uma LEF vazia e retorna um ponteiro para ela */
LEF *criaLEF();

/** Destroi a LEF e libera a memoria associada a ela, retornando NULL */
LEF *destroiLEF(LEF *lef);

/** Adiciona um evento no inicio da LEF. Faz uma copia do evento fornecido.
    Retorna 1 em caso de sucesso, 0 em caso contrario. */
int adicionaInicioLEF(LEF *lef, const Evento_t *evento);

/** Adiciona um evento na LEF de acordo com o valor do tempo logico do evento.
    Faz uma copia do evento fornecido. Retorna 1 em caso de sucesso, 0 em caso contrario. */
int adicionaOrdemLEF(LEF *lef, const Evento_t *evento);

/** Retorna e remove o primeiro evento da LEF.
    A responsabilidade de desalocar a memoria associada ao evento e do chamador. */
Evento_t *obtemPrimeiroLEF(LEF *lef);

#endif  // LEF_H

