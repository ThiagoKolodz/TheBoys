/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include "liblef.h"
#include "libset.h"
#include "liblista.h"

/*====================== DEFININDO VALORES CONSTANTES =======================*/

#define INI_LEF 0
#define T_INICIO 0                        /** Tempo inicial         **/
#define T_FIM_DO_MUNDO 525600             /** Tempo final (minutos) **/
#define N_TAMANHO_MUNDO 20000             /** Tamanho do mundo      **/
#define N_HABILIDADES 10                  /** Numero de habilidades **/
#define N_HEROIS (N_HABILIDADES * 5)      /** Numero de herois      **/
#define N_BASES (N_HEROIS / 6)            /** Numero de bases       **/
#define N_MISSOES (T_FIM_DO_MUNDO / 100)  /** Numero de missoes     **/

#define EV1 1                             /** EVENTO CHEGA   **/
#define EV2 2                             /** EVENTO ESPERA  **/
#define EV3 3                             /** EVENTO DESISTE **/
#define EV4 4                             /** EVENTO AVISA   **/
#define EV5 5                             /** EVENTO ENTRA   **/
#define EV6 6                             /** EVENTO SAI     **/
#define EV7 7                             /** EVENTO VIAJA   **/
#define EV8 8                             /** EVENTO MISSAO  **/
#define EV9 9                             /** EVENTO FIM     **/


/*========================== DEFININDO ESTRUTURAS ===========================*/

typedef struct Coord_t {

    int x;
    int y;

} Coord_t;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Base_t {
    
    int id;
    int lotacao;
    struct set_t
        *presentes; /** Utilizamos um conjunto para representar os herois presentes **/
    struct lista_t
        *espera;    /** Utilizamos uma lista para representar a fila de espera **/
    struct Coord_t local;
    
} Base_t; /** Estrutura da Base **/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Missao_t {
    
    int id;
    struct set_t *habilidades;  /** Utilizamos um conjunto para representar as **/
                                /** habilidades necessarias **/
    struct Coord_t local;
    
} Missao_t; /** Estrutura da Missao **/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Heroi_t {

    int id;
    struct set_t
        *habilidades; /** Utilizamos um conjunto para representar as habilidades **/
    int paciencia;
    int velocidade;
    int experiencia;
    int base_id;
    /* int estaVivo; Atributo para indicar se o herÂ¢i estÂ  vivo (1 para vivo, 0
       para morto) */

} Heroi_t; /** Estrutura do Heroi **/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Mundo_t {

    Base_t *bases;     /** Utilizamos o tipo Base_t **/
    Heroi_t *herois;   /** Utilizamos o tipo Heroi_t **/
    Missao_t *missoes; /** Utilizamos o tipo Missao_t **/
    int *vetHab;
    int nHabilidades;
    int tamanhoMundo;
    int nMissoes;
    int relogio;
    int nHerois;
    int nBases;
    int nMissoesConcluidas;
    int tentativasMissao;

} Mundo_t; /* Estrutura do Mundo */

/*=========================== FUNCOES AUXILIARES ============================*/

/* Implementacao da funcao 'aleat(int min, int max)', responsavel por realizar
 * o retorno de um numero aleatorio entre os parametros
 * 'min' e 'max', inclusive.
 */
int aleat(int min, int max) {
    
    /* Abaixo temos o calculo necessario para definir o intervalo
    * gerado pela funcao 'rand()'.
    */
    return min + rand() % (max - min + 1);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Funcao auxiliar para calcular a distancia cartesiana entre duas bases **/
double calcularDistancia(Coord_t primCoord, Coord_t segCoord) {
    
    int deltaX = segCoord.x - primCoord.x;
    int deltaY = segCoord.y - primCoord.y;
    return sqrt(deltaX * deltaX + deltaY * deltaY);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Funcao para transformar um array em um heap maximo. **/
void maxHeapify(int vetor1[], int vetor2[], int i, int tam) {
    
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;
    int maior = i;

    /** Verifica se o filho esquerdo eh maior que o pai **/
    if (esq < tam && vetor1[esq] > vetor1[maior])
        maior = esq;

    /** Verifica se o filho direito eh maior que o pai **/
    if (dir < tam && vetor1[dir] > vetor1[maior])
        maior = dir;

    /** Se o maior elemento nao for o pai, faz a troca e chama maxHeapify recursivamente **/
    if (maior != i) {
        
        /* Troca nos dois vetores */
        double temp1 = vetor1[i];
        vetor1[i] = vetor1[maior];
        vetor1[maior] = temp1;
        
        double temp2 = vetor2[i];
        vetor2[i] = vetor2[maior];
        vetor2[maior] = temp2;
        
        maxHeapify(vetor1, vetor2, maior, tam);
    }
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Funcao para construir um heap maximo. **/
void buildMaxHeap(int vetor1[], int vetor2[], int tam) {
    
    for (int i = tam / 2; i >= 0; i--)
        maxHeapify(vetor1, vetor2, i, tam);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** HeapSort de heap maximo. **/
void heapSort(int vetor1[], int vetor2[], int tam) {
    
    buildMaxHeap(vetor1, vetor2, tam);
    for (int i = tam - 1; i > 0; i--) {
        
        // Troca o elemento maximo (raiz) com o ultimo elemento nao ordenado
        double temp1 = vetor1[0];
        vetor1[0] = vetor1[i];
        vetor1[i] = temp1;
        
        double temp2 = vetor2[0];
        vetor2[0] = vetor2[i];
        vetor2[i] = temp2;

        // Chama maxHeapify para reconstruir o heap maximo com o restante do vetor
        maxHeapify(vetor1, vetor2, 0, i);
        
    }
    
}

/*============================ MANIPULACAO LEF ==============================*/

/** Inserir evento SAI da LEF **/
void inserirEventoSai(LEF *lef, int tempo, int heroi_id, int base_id) {

    Evento_t eve_sai;
    
    eve_sai.tipo = EV6;
    eve_sai.tempo = tempo;
    eve_sai.dado1 = heroi_id;
    eve_sai.dado2 = base_id;
    adicionaOrdemLEF(lef, &eve_sai);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Inserir evento ENTRA da LEF **/
void inserirEventoEntra(LEF *lef, int tempo, int heroi_id, int base_id) {
    
    Evento_t eve_entra;
    
    eve_entra.tipo = EV5;
    eve_entra.tempo = tempo;
    eve_entra.dado1 = heroi_id;
    eve_entra.dado2 = base_id;
    adicionaOrdemLEF(lef, &eve_entra);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Inserir evento AVISA da LEF **/
void inserirEventoAvisa(LEF *lef, int tempo, int heroi_id, int base_id) {
    
    Evento_t eve_avisa;
    
    eve_avisa.tipo = EV4;
    eve_avisa.tempo = tempo;
    eve_avisa.dado1 = heroi_id;
    eve_avisa.dado2 = base_id;
    adicionaOrdemLEF(lef, &eve_avisa);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Inserir evento ESPERA da LEF **/
void inserirEventoEspera(LEF *lef, int tempo, int heroi_id, int base_id) {
    
    Evento_t eve_espera;
    
    eve_espera.tipo = EV2;
    eve_espera.tempo = tempo;
    eve_espera.dado1 = heroi_id;
    eve_espera.dado2 = base_id;
    adicionaOrdemLEF(lef, &eve_espera);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Inserir evento VIAJA da LEF **/
void inserirEventoViaja(LEF *lef, int tempo, int heroi_id, int base_id) {
    
    Evento_t eve_viaja;
    
    eve_viaja.tipo = EV7;
    eve_viaja.tempo = tempo;
    eve_viaja.dado1 = heroi_id;
    eve_viaja.dado2 = base_id;
    adicionaOrdemLEF(lef, &eve_viaja);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Inserir evento DESISTE da LEF **/
void inserirEventoDesiste(LEF *lef, int tempo, int heroi_id, int base_id) {
    
    Evento_t eve_desiste;
    
    eve_desiste.tipo = EV3;
    eve_desiste.tempo = tempo;
    eve_desiste.dado1 = heroi_id;
    eve_desiste.dado2 = base_id;
    adicionaOrdemLEF(lef, &eve_desiste);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Inserir evento CHEGA da LEF **/
void inserirEventoChega(LEF *lef, int tempo, int heroi_id, int base_id) {
    
    Evento_t eve_chega;
    
    eve_chega.tipo = EV1;
    eve_chega.tempo = tempo;
    eve_chega.dado1 = heroi_id;
    eve_chega.dado2 = base_id;
    adicionaOrdemLEF(lef, &eve_chega);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Inserir evento MISSAO da LEF **/
void inserirEventoMissao(LEF *lef, int tempo, Missao_t *missao) {
    
    Evento_t eve_missao;
    
    eve_missao.tipo = EV8;
    eve_missao.tempo = tempo;
    eve_missao.dado1 = missao->id;
    eve_missao.dado2 = 0;
    adicionaOrdemLEF(lef, &eve_missao);
    
}

/*========================== MANIPULACAO EVENTOS ============================*/

/** Implementacao evento VIAJA **/
void eventoViaja(LEF *lef, Mundo_t *mundo, Evento_t *evento) {
    
    Heroi_t heroi = mundo->herois[evento->dado1];
    Base_t baseDestino = mundo->bases[evento->dado2];

    Base_t baseAtual = mundo->bases[heroi.base_id];
    /* Calcula duracao da viagem */
    double distancia = calcularDistancia(baseAtual.local, baseDestino.local);
    int duracao = (int) distancia / heroi.velocidade;

    /* Cria evento CHEGA para a base de destino */
    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", 
           mundo->relogio, heroi.id, baseAtual.id, baseDestino.id, 
           (int)distancia, (int)heroi.velocidade, mundo->relogio + duracao);
    inserirEventoChega(lef, mundo->relogio + duracao, heroi.id, baseDestino.id);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Implementacao evento CHEGA **/
void eventoChega(LEF *lef, Mundo_t *mundo, Evento_t *evento) {
    
    Heroi_t heroi = mundo->herois[evento->dado1];
    Base_t base = mundo->bases[evento->dado2];

    heroi.base_id = base.id;
    int tamanhoFila = base.espera->size; 
    int espera = 0;

    /** Verifica se a base est  cheia e se o tamanho da fila eh menor que a lotacao da base. **/
    if (base.lotacao > 0 && tamanhoFila < base.lotacao)
        espera = 1;
    else
        espera = (heroi.paciencia > (10 * tamanhoFila));

    /** Decide se o her¢i vai esperar na fila ou desistir. **/
    if (espera) {
        
        /** Imprime mensagem de chegada com espera. **/
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", 
               mundo->relogio, heroi.id, base.id, tamanhoFila, base.lotacao);
        
        /** Adiciona o evento de espera na lista de eventos futuros. **/
        inserirEventoEspera(lef, mundo->relogio, heroi.id, base.id);
        
    } else {
        
        /** Imprime mensagem de chegada com desistancia. **/
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", 
               mundo->relogio, heroi.id, base.id, tamanhoFila, base.lotacao);
        
        /** Adiciona o evento de desistencia na lista de eventos futuros. **/
        inserirEventoDesiste(lef, mundo->relogio, heroi.id, base.id);
        
    }
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Implementacao evento ESPERA **/
void eventoEspera(LEF *lef, Mundo_t *mundo, Evento_t *evento) {
    
    Heroi_t heroi = mundo->herois[evento->dado1];
    Base_t base = mundo->bases[evento->dado2];
    
    /** Adiciona o heroi a fila de espera da base B **/
    lista_insere(base.espera, heroi.id);
    
    /** Realiza a impressao dos dados **/
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", 
           mundo->relogio, heroi.id, base.id, base.espera->size);
    
    /** Cria evento AVISA para tratar a fila **/
    inserirEventoAvisa(lef, mundo->relogio, heroi.id, base.id);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Implementacao evento DESISTE **/
void eventoDesiste(LEF *lef, Mundo_t *mundo, Evento_t *evento) {
    
    Heroi_t heroi = mundo->herois[evento->dado1];
    
    /** Escolhe uma base destino D aleatoria **/
    int baseDestinoIndex = rand() % mundo->nBases;
    Base_t baseDestino = mundo->bases[baseDestinoIndex];
    
    /** Realiza a impressao dos dados **/
    printf("%6d: DESISTE HEROI %2d BASE %d\n", 
           mundo->relogio, heroi.id, baseDestino.id);
    
    /** Cria evento VIAJA **/
    inserirEventoViaja(lef, mundo->relogio, heroi.id, baseDestino.id);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Implementacao evento AVISA **/
void eventoAvisa(LEF *lef, Mundo_t *mundo, Evento_t *evento) {
    
    Base_t base = mundo->bases[evento->dado2];
    
    while (base.presentes->num <= base.lotacao && !lista_vazia(base.espera)) {
        
        int idHeroiEspera = 0;
        /** Retira o primeiro heroi da fila de B **/
        lista_retira(base.espera, &idHeroiEspera);
        Heroi_t heroi = mundo->herois[idHeroiEspera];

        /** Adiciona o heroi ao conjunto de herois presentes em B **/
        set_adicionar(base.presentes, idHeroiEspera);
        
        /** Realiza a impressao dos dados **/
        printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA ", 
               mundo->relogio, base.id, base.presentes->num, base.lotacao);
        lista_imprime(base.espera);
        
        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", 
               mundo->relogio, base.id, heroi.id);
        
        /** Cria evento ENTRA **/
        inserirEventoEntra(lef, mundo->relogio, heroi.id, base.id);
        
    }
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Implementacao evento ENTRA **/
void eventoEntra(LEF *lef, Mundo_t *mundo, Evento_t *evento) {
    
    Heroi_t heroi = mundo->herois[evento->dado1];
    Base_t base = mundo->bases[evento->dado2];

    /** Calcula TPB = tempo de permanencia na base **/
    int TPB = 15 + heroi.paciencia * (1 + rand() % 20);
    
    /** Realiza a impressao dos dados **/
    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", 
           mundo->relogio, heroi.id, base.id, base.presentes->num, 
           base.lotacao, mundo->relogio + TPB);
    
    /* Cria evento SAI */
    inserirEventoSai(lef, mundo->relogio + TPB, heroi.id, base.id);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Implementacao evento SAI **/
void eventoSai(LEF *lef, Mundo_t *mundo, Evento_t *evento) {
    
    Heroi_t heroi = mundo->herois[evento->dado1];
    Base_t base = mundo->bases[evento->dado2];
    /** Escolhe uma base destino D aleatoria **/
    int baseDestinoIndex = rand() % mundo->nBases;
    Base_t baseDestino = mundo->bases[baseDestinoIndex];
    
    /** Remove o heroi do conjunto de herois presentes na base **/
    set_remover(base.presentes, heroi.id);

    /** Realiza a impressao dos dados **/
    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", 
           mundo->relogio, heroi.id, base.id, 
           base.presentes->num, base.lotacao);
    
    /* Cria evento VIAJA */
    inserirEventoViaja(lef, mundo->relogio, heroi.id, baseDestino.id);
    
    /* Realiza a impressao dos dados */
    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [", 
           mundo->relogio, base.id, base.presentes->num, base.lotacao);
    
    for (int i = 0; i < base.espera->size; i++) {
        
        int idHeroiFila;
        lista_consulta(base.espera, &idHeroiFila, i);
        printf(" %2d", idHeroiFila);
        
        if (i < base.espera->size - 1)
            printf(" ");
        
    }
    printf(" ]\n");
    
    /* Cria evento AVISA */
    inserirEventoAvisa(lef, mundo->relogio, heroi.id, base.id);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void eventoMissao(LEF *lef, Mundo_t *mundo, Evento_t *evento) {
    
    /** Obter a missao associada ao evento **/
    Missao_t *missao = &mundo->missoes[evento->dado1];
    
    /** Variaveis para armazenar distancias, identificadores de base, e habilidades por base **/
    int *distancias, *id_bases, melhor_base;
    struct set_t **habilidades_bases;
    
    /** Exibir informacoes sobre a missao e habilidades necessarias **/
    printf("%6d:  MISSAO %d HAB REQ: ", mundo->relogio, missao->id);
    set_imprimir(missao->habilidades);

    /** Incrementar o numero de tentativas de missao **/
    (mundo->tentativasMissao)++;

    /** Alocar memoria para os arrays **/
    distancias = malloc(mundo->nBases * sizeof(int));
    id_bases = malloc(mundo->nBases * sizeof(int));
    habilidades_bases = malloc(mundo->nBases * sizeof(struct set_t *));

    /** Verificar se a alocacao de memoria foi bem-sucedida **/
    if (!distancias || !id_bases || !habilidades_bases) {
        
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(EXIT_FAILURE);
        
    }
    
    /** Inicializar identificadores de base **/
    for (int i = 0; i < mundo->nBases; i++)
        id_bases[i] = i;
    
    /** Calcular distancias e ordenar bases por distancia usando HeapSort **/
    for (int i = 0; i < mundo->nBases; i++)
        distancias[i] = calcularDistancia(missao->local, mundo->bases[i].local);
    
    heapSort(distancias, id_bases, mundo->nBases);
    
    /** Iterar sobre as bases ordenadas para obter habilidades por base **/
    for (int i = 0; i < mundo->nBases; i++) {
        
        habilidades_bases[i] = set_criar(mundo->nHabilidades + 1);
        for (int j = 0; j < N_HEROIS; j++)
            if (set_pertence(mundo->bases[id_bases[i]].presentes, j))
                set_uniao(mundo->herois[j].habilidades, habilidades_bases[i], habilidades_bases[i]);
        
        /** Exibir informacoes sobre as habilidades da base **/
        printf("%6d:  MISSAO %d HAB BASE %2d: ", mundo->relogio, missao->id, id_bases[i]);
        set_imprimir(habilidades_bases[i]);
        
    }

    /** Encontrar a base adequada para a missao **/
    melhor_base = -1;
    int indice_base = 0;
    while ((indice_base < mundo->nBases) && (melhor_base == -1)) {
        
        if (set_contem(habilidades_bases[indice_base], missao->habilidades))
            melhor_base = id_bases[indice_base];
        indice_base++;
        
    }

    /** Resolver a missao **/
    if (melhor_base != -1) {
        
        /** Incrementar o numero de missoes concluidas **/
        (mundo->nMissoesConcluidas)++;
        
        /** Incrementar a experiencia dos herois na base **/
        for (int j = 0; j < mundo->nHerois; j++)
            if (set_pertence(mundo->bases[melhor_base].presentes, j))
                (mundo->herois[j].experiencia)++;
        
        /** Exibir informacoes sobre a missao cumprida **/
        printf("%6d:  MISSAO %d CUMPRIDA BASE %2d HEROIS: ", mundo->relogio, missao->id, melhor_base);
        set_imprimir(mundo->bases[melhor_base].presentes);
        
    } else {
        
        /** Exibir mensagem se a missao for impossivel e agendar uma nova tentativa para o proximo dia **/
        printf("%6d:  MISSAO %d IMPOSSIVEL\n", mundo->relogio, missao->id);
        inserirEventoMissao(lef, evento->tempo + 24 * 60, missao);
        
    }

    /** Liberar memoria alocada **/
    for (int i = 0; i < mundo->nBases; i++)
        set_destruir(habilidades_bases[i]);
    
    free(habilidades_bases);
    free(distancias);
    free(id_bases);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Implementacao evento FIM **/
void eventoFim(LEF *lef, Mundo_t *mundo, Evento_t *evento) {
    
    printf("%6d: FIM\n", mundo->relogio);

    for (int i = 0; i < mundo->nHerois; i++) {
        
        printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", mundo->herois[i].id, 
               mundo->herois[i].paciencia, mundo->herois[i].velocidade, 
               mundo->herois[i].experiencia);
        set_imprimir(mundo->herois[i].habilidades);
        
    }

    /** Calcular e imprimir estatisticas finais **/
    printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n",
           mundo->nMissoesConcluidas, mundo->nMissoes, 
           ((double)mundo->nMissoesConcluidas / mundo->nMissoes) * 100, 
           ((double)mundo->tentativasMissao / mundo->nMissoes));
    
}

/*========================= MANIPULACAO SIMULACAO ===========================*/

/* Cria um cojunto de elementos aleatorios a partir de um vetor de valores com 
 * tamanhao aleatorio definido entre os valores 'min' e 'max'.
 */
struct set_t * cria_cjt_hab(int min, int max, int *vetHab) {
    
    int indVet;
    int numHab = aleat(min, max);
    int tamVet = N_HABILIDADES;
    struct set_t *conj_hab;
    
    /** Cria um conjunto vazio com espaco para N_HABILIDADES + 1 elementos. **/
    conj_hab = set_criar(N_HABILIDADES + 1); 
    
    for (int i = 0; i < numHab; i++) {
        
        /** Escolhe um indice aleatorio no vetor e adiciona o valor correspondente ao conjunto. **/
        indVet = aleat(0, tamVet - 1);
        set_adicionar(conj_hab, vetHab[indVet]);
        tamVet--;
        
        /** Troca o valor escolhido com o ultimo valor nao escolhido no vetor. **/
        int aux = vetHab[indVet];
        vetHab[indVet] = vetHab[tamVet];
        vetHab[tamVet] = aux;
        
    }
    
    return conj_hab;
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Gera os herois e os retorna em um vetor. **/
Heroi_t *cria_herois(Mundo_t *mundo){
    
    Heroi_t *vet_herois = malloc(sizeof(Heroi_t) * mundo->nHerois);

    for (int i = 0; i < mundo->nHerois; i++){
        
        vet_herois[i].id = i;
        vet_herois[i].experiencia = 0;
        vet_herois[i].paciencia = aleat(0, 100);
        vet_herois[i].velocidade = aleat(50, 5000); // em metros/minuto = 3 Km/h a 300 Km/h
        vet_herois[i].habilidades = cria_cjt_hab(1, 3, mundo->vetHab);
        
    }

    return vet_herois;
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Gera as bases e as retorna em um vetor. **/
Base_t *cria_bases(Mundo_t *mundo) {
    
    Base_t *vet_base = malloc(sizeof(Base_t) * mundo->nBases);
    
    for (int i = 0; i < mundo->nBases; i++) {
        
        vet_base[i].id = i;
        vet_base[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
        vet_base[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
        vet_base[i].lotacao = aleat(3, 10);
        vet_base[i].presentes = set_criar(mundo->nHerois);
        vet_base[i].espera = lista_cria();
        
    }
    
    return vet_base;
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Gera as missoes e as retorna em um vetor. **/
Missao_t *cria_missoes(Mundo_t *mundo) {
    
    Missao_t *vet_missao = malloc(sizeof(Missao_t) * mundo->nMissoes);
    
    for (int i = 0; i < mundo->nMissoes; i++) {
        
        vet_missao[i].id = i;
        vet_missao[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
        vet_missao[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
        vet_missao[i].habilidades = cria_cjt_hab(6, 10, mundo->vetHab);
        
    }
    
    return vet_missao;
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Funcao para criar o mundo virtual **/
Mundo_t *genesis() {
    
    /** Aloca espaco para a estrutura Mundo_t **/
    Mundo_t *mundo = (Mundo_t *)malloc(sizeof(Mundo_t));
    
    /** Inicializa os campos da estrutura Mundo_t **/
    mundo->vetHab = malloc(sizeof(int) * N_HABILIDADES);
    for (int i = 0; i < N_HABILIDADES; i++)
        mundo->vetHab[i] = i + 1;
    
    mundo->relogio = T_INICIO;
    mundo->nHerois = N_HEROIS;
    mundo->nHabilidades = N_HABILIDADES;
    mundo->tamanhoMundo = N_TAMANHO_MUNDO;
    mundo->nBases = N_BASES;
    mundo->nMissoes = N_MISSOES;
    mundo->nMissoesConcluidas = T_INICIO;
    mundo->tentativasMissao = T_INICIO;
    mundo->herois = cria_herois(mundo);
    mundo->missoes = cria_missoes(mundo);
    mundo->bases = cria_bases(mundo);
    
    return mundo;
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Funcao para inicializar o mundo virtual **/
void sete_dias(LEF *lef, Mundo_t *mundo) {

    Evento_t eve_fim;
    
    /** Gera eventos de chegada para todos os herois. **/
    for (int i = 0; i < mundo->nHerois;  i++) {
        
        int tempo = aleat(0, 4320); /** 4320 = 60*24*3 = 3 dias **/
        mundo->herois[i].base_id = aleat(0, N_BASES - 1);
        inserirEventoChega(lef, tempo, mundo->herois[i].id, 
                           mundo->herois[i].base_id);
        
    }
    
    for (int i = 0; i < mundo->nMissoes; i++) {
        
        int tempo = aleat(0, T_FIM_DO_MUNDO);
        inserirEventoMissao(lef, tempo, &mundo->missoes[i]);
        
    }

    eve_fim.tempo = T_FIM_DO_MUNDO;
    eve_fim.tipo = EV9;
    eve_fim.dado1 = 0;
    eve_fim.dado2 = 0;

    adicionaOrdemLEF(lef, &eve_fim);   

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Funcao para destruir um heroi **/
void destroi_heroi(Heroi_t *heroi) {
    
    set_destruir(heroi->habilidades);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Funcao para destruir um base **/
void destroi_base(Base_t *base) {
    
    set_destruir((base->presentes));
    lista_destroi((base->espera));
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Funcao para destruir um missao **/
void destroi_missao(Missao_t *missao) {
    
    set_destruir(missao->habilidades);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Funcao para destruir o mundo **/
void apocalipse(Mundo_t *mundo) {
    
    /** Destroi os herois **/
    for (int i = 0; i < mundo->nHerois; i++)
        destroi_heroi(&mundo->herois[i]);
    free(mundo->herois);
    
    /** Destroi as bases **/
    for (int i = 0; i < mundo->nBases; i++)
        destroi_base(&mundo->bases[i]);
    free(mundo->bases);
    
    /** Destroi as missoes **/
    for (int i = 0; i < mundo->nMissoes; i++)
        destroi_missao(&mundo->missoes[i]);
    free(mundo->missoes);
    
    /** Finalmente, destroi a estrutura do mundo */
    free(mundo->vetHab);
    free(mundo);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main() {
    
    srand(time(NULL));
    Mundo_t *mundo = genesis();
    LEF *lef = criaLEF();
    Evento_t *evento;
    /** Referencia ao nome da batalha do final do mundo **/
    int armagedom = 0;
    
    /** Referencia ao tempo de criacao do mundo segundo o Cristianismo **/
    sete_dias(lef, mundo);
    
    while (!armagedom) {
        
        evento = obtemPrimeiroLEF(lef);
        mundo->relogio = evento->tempo;
        
        switch (evento->tipo) {
            case EV1:
                eventoChega(lef, mundo, evento);
                break;
            case EV2:
                eventoEspera(lef, mundo, evento);
                break;
            case EV3:
                eventoDesiste(lef, mundo, evento);
                break;
            case EV4:
                eventoAvisa(lef, mundo, evento);
                break;
            case EV5:
                eventoEntra(lef, mundo, evento);
                break;
            case EV6:
                eventoSai(lef, mundo, evento);
                break;
            case EV7:
                eventoViaja(lef, mundo, evento);
                break;
            case EV8:
                eventoMissao(lef, mundo, evento);
                break;
            case EV9:
                eventoFim(lef, mundo, evento);
                armagedom = 1;
            break;
        }
         
        free(evento);
        
    }
    
    destroiLEF(lef);
    /** Referencia ao nome dado ao fim do mundo segundo o Cristianismo **/
    apocalipse(mundo);
    
    return 0;
    
}
