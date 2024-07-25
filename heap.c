/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* Troca dois enderecos de posicao entre si. */
void changePos(double *ini, double *fim) {
    
    double temp = *ini;
    *ini = *fim;
    *fim = temp;
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* Função para transformar um array em um heap máximo. */
void maxHeapify(double vetor[], int i, int tam, long *comp) {
    
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;
    int maior = i;

    // Verifica se o filho esquerdo é maior que o pai
    if ((*comp)++, esq < tam && vetor[esq] > vetor[maior])
        maior = esq;

    // Verifica se o filho direito é maior que o pai
    if ((*comp)++, dir < tam && vetor[dir] > vetor[maior])
        maior = dir;

    // Se o maior elemento não for o pai, faz a troca e chama maxHeapify recursivamente
    if ((*comp)++, maior != i) {
        
        changePos(&vetor[i], &vetor[maior]);
        maxHeapify(vetor, maior, tam, comp);
        
    }
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* Função para construir um heap máximo. */
void buildMaxHeap(double vetor[], int tam, long *comp) {
    
    for (int i = tam / 2; i >= 0; i--)
        maxHeapify(vetor, i, tam, comp);
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

long heapSort(double vetor[], int tam) {
    
    long comp = 0;
    buildMaxHeap(vetor, tam, &comp);

    for (int i = tam - 1; i > 0; i--) {
        
        // Troca o elemento máximo (raiz) com o último elemento não ordenado
        changePos(&vetor[0], &vetor[i]);
        
        // Chama maxHeapify para reconstruir o heap máximo com o restante do vetor
        maxHeapify(vetor, 0, i, &comp);
        
    }

    return comp;
    
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

