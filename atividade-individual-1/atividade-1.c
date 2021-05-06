#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_THREADS 6
#define ORDEM_MATRIX 6


// Variaveis Compartilhadas
int controleVertCompart = 0;                               // Variavel compartilhada que determina qual vertice deve ser lido.
int controleDeResultados = 0;                              // Variavel para controlar a finalização do processamento da matrix de resultados.
int matrizDeResultadosCompart[6] = {0, 0, 0, 0, 0, 0};     // Variavel compartilhada para armazenamento dos resultador processador.
int matrizCompart[ORDEM_MATRIX][ORDEM_MATRIX];             // Matrix compartilhada de dados.

// Criando MUTEX das variaveis compartilhadas
pthread_mutex_t mutexControleVertCompart;
pthread_mutex_t mutexControleDeResultados;
pthread_mutex_t mutexMatrizDeResultadosCompart;
pthread_mutex_t mutexMatrizCompart;

/*
* Função para criação da matriz dado um array de elementos.
* elements: array de elementos da matriz.
* ORDEM_MATRIX: indica a ORDEM_MATRIX da matriz.
*/
void makeMatriz(int elements[] ,int ordem) {
    int i, j, k;
    k = 0;

    printf ("\nIniciando a criacao da matriz \n\n");

    for ( i=0; i < ordem; i++ ) {
        for ( j=0; j < ordem; j++ ) {
        matrizCompart[i][j] = elements[k];

        // printf ("\nElemento[%d][%d] = %d", i, j, matrizCompart[i][j]);
        printf ("[%d]", matrizCompart[i][j]);
        k++;
        }
        printf ("\n");
    }
}

void *calcularGrauDaMatriz(void *matriz) {
    // Buscar um vertice para ser processado.
    pthread_mutex_lock (&mutexControleVertCompart); // inicio da seção crítica
        printf("######\t INICIO DA SESSAO CRITICA:: Obtendo valor do vertice %d \t######\n", controleVertCompart);
        int vertice = controleVertCompart;

    // Atualizar vertice a ser processamento.
    controleVertCompart++;
        printf("######\t FIM DA SESSÃO CRITICA:: atualizando vertice %d \t######\n", controleVertCompart);
        pthread_mutex_unlock (&mutexControleVertCompart); // fim da seção crítica

    // Processar a contagem de valores diferentes de 0 no vertice
    int i, j, resultado = 0;
    for (i=vertice; i == vertice; i++) {
        for (j=0; j < ORDEM_MATRIX; j++) {
            // printf("Processando Vertice: %d\n", vertice);
            // printf("[%d]\n", matrizCompart[i][j]);
            if(matrizCompart[i][j] != 0) {
                resultado++;
            }
        }
    }

    printf ("o grau do vertice %d é: %d\n", vertice, resultado);

    // Armazenar a contagem na variavel compartilhada de resultado
    // Sessão critica para a Matrix de Resultados
    pthread_mutex_lock (&mutexMatrizDeResultadosCompart);
    matrizDeResultadosCompart[vertice] = resultado;
    pthread_mutex_unlock (&mutexMatrizDeResultadosCompart);

    // atualiza o controle de resultados
    pthread_mutex_lock (&mutexControleDeResultados);
    controleDeResultados++;
    pthread_mutex_unlock (&mutexControleDeResultados);

    pthread_exit(NULL);
}

void getMaiorGrau() {
    printf("\n\n Matrix de Resultados:"); 
    int i = 0;
    for (int i = 0; i < 6; i++) {
        printf("[%d]", matrizDeResultadosCompart[i]);
    }

    int numeroMax;

    // Busca o Maior Elemento
    numeroMax = matrizDeResultadosCompart[0];
    for (i = 0;i < NUM_THREADS; i++) {
        if (matrizDeResultadosCompart[i] > numeroMax)
            numeroMax = matrizDeResultadosCompart[i];
    }


    printf("\n\n");    
    for (i = 0;i < NUM_THREADS; i++) {
        char letraVertice[10];
        if(numeroMax == matrizDeResultadosCompart[i]) {
            // Define a letra.
            switch (i) {
                case 0:
                    strcpy(letraVertice, "A");
                    break;
                case 1:
                    strcpy(letraVertice, "B");
                    break;
                case 2:
                    strcpy(letraVertice, "C");
                    break;
                case 3:
                    strcpy(letraVertice, "D");
                    break;
                case 4:
                    strcpy(letraVertice, "E");
                    break;
                case 5:
                    strcpy(letraVertice, "F");
                    break;
            
                default:
                    break;
            }

            // Imprime o Resultado
            printf("\n O Vertice %s possui a maior quantidade de arestas, tendo grau %d.", letraVertice, matrizDeResultadosCompart[i]);
        }
    }

} 



int main(int argc, char *argv[]) {
    int array[36] = { 0, 1, 0, 0, 2, 0, 1, 0, 3, 0, 2, 0, 0, 3, 0, 6, 0, 0, 0, 0, 6, 0, 4, 3, 2, 2, 0, 4, 0, 0, 0, 0, 0, 3, 0, 0 };
    makeMatriz(array, ORDEM_MATRIX);

    // Inicia as variaveis MUTEX
    pthread_mutex_init(&mutexControleVertCompart, NULL);
    pthread_mutex_init(&mutexControleDeResultados, NULL);
    pthread_mutex_init(&mutexMatrizDeResultadosCompart, NULL);
    pthread_mutex_init(&mutexMatrizCompart, NULL);


    // Criar Threads Paralelas
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    printf("\n\n"); 
    for (t = 0; t < NUM_THREADS; t++) {
        printf("Função Main: Criando Thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, calcularGrauDaMatriz, (void *) &matrizCompart);
        
        // Caso ocorra erro na criação da Thread
        if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
        }
    }
    printf("\n\n"); 

    // Processar Matriz de Resultados para Obter o Maior Grau.
    // printf("%d", controleDeResultados);
    // co
    while (controleDeResultados < 7) {
        if(controleDeResultados == 6) {
            getMaiorGrau();
            break;
        }
    }
    


    // Destroi as Variaveis MUTEX
    pthread_mutex_destroy(&mutexControleVertCompart);
    pthread_mutex_destroy(&mutexControleDeResultados);
    pthread_mutex_destroy(&mutexMatrizDeResultadosCompart);
    pthread_mutex_destroy(&mutexMatrizCompart);

    pthread_exit (NULL);

    return 0;
}