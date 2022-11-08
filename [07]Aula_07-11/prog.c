#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

#define QTD 5

int count = 0;

void tarefa(long i){
    sleep(i+1);
}

void *barreira(void*p){
    long index = (long) p;
    while(1){
        printf("[%ld] Vou realizar tarefa\n", index);
        tarefa(index);
        atomic_fetch_add(&count, 1);
        printf("Ola\n");
        while ((count % QTD) != 0);
        printf("[%ld] realizou tarefa %d\n", index, count);
    }
}

int main(){
    pthread_t threads[QTD];

    for (long i = 0; i < QTD; i++)
    {
        pthread_create(&threads[i], NULL, barreira, (void*)i);
    }

    for (long i = 0; i < QTD; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}