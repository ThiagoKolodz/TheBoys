#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "libset.h"

struct set_t *set_criar(int max) {
    struct set_t *novoConjunto = (struct set_t *)malloc(sizeof(struct set_t));
    if (novoConjunto != NULL) {
        
        novoConjunto->tamanho = max;
        novoConjunto->num = 0;
        novoConjunto->flag = (bool *)malloc(max * sizeof(bool));
        
        if (novoConjunto->flag == NULL) {
            
            free(novoConjunto);
            return NULL;
            
        }
        for (int i = 0; i < max; i++) {
            
            novoConjunto->flag[i] = false;
            
        }
    }
    return novoConjunto;
}

struct set_t *set_destruir(struct set_t *s) {
    if (s != NULL) {
        free(s->flag);
        free(s);
    }
    return NULL;
}

int set_adicionar(struct set_t *s, int item) {
    
    if (s == NULL || item < 0 || item >= s->tamanho|| s->flag[item]) {
        return 0; // Falha
    }
    
    s->flag[item] = true;
    s->num++;
    return 1; // Sucesso
     
}

int set_remover(struct set_t *s, int item) {
    if (s == NULL || item < 0 || item >= s->tamanho|| !s->flag[item]) {
        return 0; // Falha
    }

    s->flag[item] = false;
    s->num--;
    return 1; // Sucesso
}

int set_pertence(struct set_t *s, int item) {
    if (s == NULL || item < 0 || item >= s->tamanho) {
        return 0; // Falha
    }

    if (s->flag[item] == true)
      return 1;
    
    return 0;
}

int set_vazio(struct set_t *s) {
    return (s == NULL || s->num == 0);
}

int set_cardinalidade(struct set_t *s) {
    if (s == NULL || s->flag == NULL)
        return 0; // Conjunto inv†lido

    int cardinalidade = 0;
    for (int i = 0; i < s->tamanho; i++)
        if (s->flag[i] == 1)
            cardinalidade++;
    
    return cardinalidade;
}

int set_contem(struct set_t *s1, struct set_t *s2) {
    struct set_t *aux;

    if (!s1 || !s2)
        return 0;

    aux = set_criar(s2->tamanho);
    set_intersecao(s1, s2, aux);
    aux->num = set_cardinalidade(aux);

    if (set_igual(s2, aux)) {
        
        set_destruir(aux);
        return 1;
        
    }
   
    set_destruir(aux);
    return 0;
}

int set_igual(struct set_t *s1, struct set_t *s2) {
    int i, tam;
    bool igual;

    // caso suas cardinalidades forem diferentes, eles nao podem ser iguais
    if (s1->num != s2->num)
      return (0); 

    // escolhemos a menor das capacidades para ser o tamanho que
    // usaremos no laco para nao acessarmos posicoes invalidas
    // ao comparar os bitmaps   
    if (s1->tamanho > s2->tamanho)
      tam = s2->tamanho;
    else 
      tam = s1->tamanho;  

    i = 0;
    igual = 1;
    while (i < tam && igual)
    {
      if (s1->flag[i] != s2->flag[i])
         igual = 0;
      i++;     
    }

    if (igual)
      return 1;

    return 0;
}

int set_uniao(struct set_t *s1, struct set_t *s2, struct set_t *s3) {
    if (s1 == NULL || s2 == NULL || s3 == NULL || s1->flag == NULL || s2->flag == NULL || s3->flag == NULL)
        return 0; // Falha

    // Verificar se os conjuntos de destino tàm espaáo suficiente
    if (s3->tamanho < s1->tamanho || s3->tamanho < s2->tamanho)
        return 0; // Falha - Espaáo insuficiente para armazenar a uni∆o

    // Realizar a uni∆o dos conjuntos
    for (int i = 0; i < s3->tamanho; i++)
        s3->flag[i] = (i < s1->tamanho ? s1->flag[i] : 0) || (i < s2->tamanho ? s2->flag[i] : 0);

    s3->num = set_cardinalidade(s3);

    return 1; // Sucesso
}

int set_intersecao(struct set_t *s1, struct set_t *s2, struct set_t *s3) {
    if (s1 == NULL || s2 == NULL || s3 == NULL || 
        s1->tamanho != s2->tamanho || s1->tamanho != s3->tamanho) {
        return 0; // Falha
    }

    for (int i = 0; i < (s3->tamanho); i++)
      s3->flag[i] = (s1->flag[i] && s2->flag[i]);

    s3->num = set_cardinalidade(s3);

    return 1;
    
}

int set_diferenca(struct set_t *s1, struct set_t *s2, struct set_t *s3) {
    if (s1 == NULL || s2 == NULL || s3 == NULL || 
        s1->tamanho != s2->tamanho || s1->tamanho != s3->tamanho) {
        return 0; // Falha
    }

    for (int i = 0; i < s1->tamanho; i++) {
        s3->flag[i] = s1->flag[i] && !s2->flag[i];
    }

    return 1; // Sucesso
}

int set_copiar(struct set_t *s1, struct set_t *s2) {
    if (s1 == NULL || s2 == NULL || s1->tamanho != s2->tamanho) {
        return 0; // Falha
    }

    for (int i = 0; i < s1->tamanho; i++) {
        s2->flag[i] = s1->flag[i];
    }

    return 1; // Sucesso
}

void set_imprimir(struct set_t *s) {
    if (s != NULL) {
        printf("[ ");
        for (int i = 0; i < s->tamanho; i++) {
            if (s->flag[i]) {
                printf("%d ", i);
            }
        }
        printf("] \n");
    }
}

