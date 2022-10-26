#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

#define QTD 5

int global = 0;
int lock = 0;

int test_and_set(int* var){
    int aux = *var;
    *var = 1;

    return aux;
}

void* funcao_thread(void* p) {
	int i = 0;

    while(i < 10){
        while(atomic_flag_test_and_set_explicit(&lock,0)){
            global++;
            i++;
            lock = 0;
        }
    }
    
}

int main(void) {
	pthread_t ths[QTD];
	void* ret[QTD];

	printf("[main] start\n");

	for (long i = 0; i < QTD; i++)
		pthread_create(&ths[i], 0,
		  funcao_thread,(void*)i);

	for (int i = 0; i < QTD; i++) {
	       pthread_join(ths[i], &ret[i]);
	}

	printf("[main] end global = %d\n", global);

	return 0;
}
