/* 
* Enunciado:
* Elabore um programa em qualquer linguagem de programação que evidencie a 
* concorrência entre três instâncias de uma thread cujo objetivo
* é apenas imprimir seu identificador (ID). 
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 3

/*
* Função passada como parametro para ser executada nas Threads Concorrentes.
* Imprime o identificados ID da Thread.
*/
void *printThreadId(void *threadid) {
  long tid;
  tid = (long) threadid;
  printf("Thread Concorrente de ID: #%ld\n", tid);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;

  // Loop para criação das Threads Concorrentes
  for (t = 0; t < NUM_THREADS; t++) {
    // Criando Treads Concorrentes baseado na contante de NUM_THREADS.
    rc = pthread_create(&threads[t], NULL, printThreadId, (void *) t);

    if (rc) {
      printf("ERROR: Ocorreu um erro ao criar a Thread. O codigo de pthread_create() é %d\n", rc);
      exit(-1);
    }
  }

  /* Last thing that main() should do */
  pthread_exit(NULL);
}