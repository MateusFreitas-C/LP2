#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define QTD 50

void* funcao_thread(void* p) {
	long id = (long) p;
    long val = rand() % 100;
	printf("NOVA THREAD #%ld\n",id);  //Função executada pela thread.
	pthread_exit((void*)val);
}

int main(void) {
	pthread_t ths[QTD];
	void* ret[QTD];
    double tempo;

	clock_t inicio = clock(); //inicio da contagem

	for (long i = 0; i < QTD; i++){
        pthread_create(&ths[i], 0,
		  funcao_thread,(void*)i); //criação da thread
    }
		

	for (int i = 0; i < QTD; i++) {
	       pthread_join(ths[i], &ret[i]); // execução da thread
	}

	clock_t fim = clock(); //fim da contagem
    tempo += (double)(fim - inicio) / CLOCKS_PER_SEC; //obtenção do resultado em segundos
	printf("O tempo gasto na criação das Threads foi de %f segundos.\n", tempo);
	return 0;
}