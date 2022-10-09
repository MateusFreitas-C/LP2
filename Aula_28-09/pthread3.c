#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define QTD 5

pthread_mutex_t mutex;

long var_global;

void* funcao_thread(void* p) {
	long id = (long) p;
	long val = rand() % 100;
	printf("[thread #%ld] start\n",id);
//	sleep(1+(int)id);
	printf("[thread #%ld] %ld\n", id, val);
	int limit =  0;

	do {
		pthread_mutex_lock(&mutex);
		var_global++;
		pthread_mutex_unlock(&mutex);
		limit++;

	} while (limit < 1000000);
	
	printf("[thread #%ld] var_global = %ld\n", 
			id, var_global);

	printf("thread #%ld] end\n", id);	
	pthread_exit((void*)val);
}

int main(void) {
	pthread_t ths[QTD];
	void* ret[QTD];

	time_t t;

	srand(time(&t));

	var_global = 0;

	pthread_mutex_init(&mutex, 0);

	printf("[main] start\n");

	for (long i = 0; i < QTD; i++)
		pthread_create(&ths[i], 0,
		  funcao_thread,(void*)i);

	for (int i = 0; i < QTD; i++) {
	       pthread_join(ths[i], &ret[i]);
	       printf("[main] thread #%d: %ld\n", 
			       i, (long)ret[i]);
	}

	printf("[main] var_global = %ld\n", var_global);
	printf("[main] end\n");

	return 0;
}
