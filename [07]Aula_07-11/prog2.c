#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

#define QTD 5

int arrive[QTD];
int proceed[QTD];

void tarefa(long i){
    sleep(i+1);
}

void *barreira(void*p){
    long index = (long) p;
    while(1){
        printf("[%ld] Vou realizar tarefa\n", index);
        tarefa(index);
        arrive[index] = 1;
        while (proceed[index] != 1);
        proceed[index] = 0;
        printf("[%ld] realizou tarefa\n", index);
        
    }
}

void* coordinator(void *p){
    while(1){

        for (int i = 0; i < QTD; i++){
            while(arrive[i] != 1);
            arrive[i] = 0;

        }

        for (int i = 0; i < QTD; i++){

            proceed[i] = 1;
        }
        
    }
}

int main(){
    pthread_t threads[QTD];
    pthread_t coordinator_thread;

    for (int i = 0; i < QTD; i++){

        arrive[i] = 0;
        proceed[i] = 0;
    }
    

    for (long i = 0; i < QTD; i++)
    {
        pthread_create(&threads[i], NULL, barreira, (void*)i);
    }

    pthread_create(&coordinator_thread, NULL, coordinator, NULL);

    for (long i = 0; i < QTD; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}