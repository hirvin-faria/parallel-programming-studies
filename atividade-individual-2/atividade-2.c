/* para compilar este programa execute:
gcc-11 atividade-2.c -fopenmp -o atividade-2.out

para executar ./atividade-2.out

no mac sera nescessario instalar uma nova versao do compilador gcc e executar gcc-NUMERO_DA_VERSAO
pois o gcc padrao da apple nao possui suporte ao omp.
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<time.h>

int matrizCompart[6][6];
int verifMEsparsa = 0;
int linhaDaMatriz = 0;
int ordem = 0;

// Funcao para criacao da matriz dado um array de elementos.
void geraMatriz(int elementos[]) {
    int i, j, k;
    k = 0;
    
    printf ("\nIniciando a criacao da matriz \n\n");

    for ( i=0; i < ordem; i++ ) {
        for ( j=0; j < ordem; j++ ) {
            matrizCompart[i][j] = elementos[k];

            printf ("[%d]", matrizCompart[i][j]);
            k++;
        }
        printf ("\n");
    }
}

// Funcao se determina se uma matriz e esparsa ou nao.
void verificaMatrizEsparsa() {
        
    int totalElementosMatriz = pow(ordem, 2);
    int quantMaxPMatEsparsa = totalElementosMatriz/2;

    if(quantMaxPMatEsparsa < verifMEsparsa) {
        printf("\n\n MATRIZ E ESPARSA");
    } else {
        printf("\n\n MATRIZ NAO E ESPARSA");
    }
}

int main(int argc, char *argv[]) {
     
    printf("--------------------------------------------------------------------------------\n");
    printf("| Trabalho A2 - Programacao Paralela - Questão 1A                              |\n");
    printf("|                                                                              |\n");
    printf("| Este programa verifica se uma matriz gerada aleatoriamente e esparsa ou não, |\n");
    printf("| uma matriz esparsa e aquela cuja maioria dos elementos possuem o valor 0     |\n");
    printf("--------------------------------------------------------------------------------\n");


    // Obtem a ordem da matriz a ser gerada
    printf("Para prosseguir digite a Ordem da Matriz a ser gerada para teste: ");
    scanf("%d", &ordem);
    int maxElementosVetor = pow(ordem, 2);


    int elementosDoVetorFormador[maxElementosVetor];
    
    // Gera um vetor com numeros aleatorios entre 0 e 1
    srand(time(NULL));
    for (int i = 0; i < maxElementosVetor; i++) {
        elementosDoVetorFormador[i] = rand() % 2; 
    }
    
    // Funcao que gera a matriz
    geraMatriz(elementosDoVetorFormador);


    // Bloco de Codigo Paralelo para processamento da matriz esparsa
    #pragma omp parallel default(shared) num_threads(ordem)
    {
        // Obtém o id da thread
        int tid = omp_get_thread_num(); 

        int elemento, linha;        
        int j = 0;

        // Obtem acesso a linha da matriz e atualiza a linha a ser processada
        linha = linhaDaMatriz;
        linhaDaMatriz++;

        // Percorre a linha e verifica se o elemento e diferente de 0 ou nao
        for (j=0; j < ordem; j++) {
            elemento = matrizCompart[linha][j];
            if(elemento == 0) {
                verifMEsparsa++;
            }
            printf ("\nThread ID: %d | Elemento[%d][%d] | Verificação de Matriz Esparsa: %d", tid, linha, j, verifMEsparsa);
        }
    }

    // Funcao para verificar matriz esparsa
    verificaMatrizEsparsa();
    
    return 0;
}