//SC = Seção Crítica

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define QTD 4

sem_t mutex;
int var = 0;

void* secao_critica(void* p){
    long index = (long)p;

    printf("[%ld] iniciou\n", index);
    while (1){
        sem_wait(&mutex);
        printf("[%ld]seção crítica\n", index);
        var++;
        printf("[%ld] = %d\n", index, var);
        sem_post(&mutex);
        sleep(1);
    }
}

int main(){
    pthread_t threads[QTD];

    sem_init(&mutex, 0, 1);

    for (long i = 0; i < QTD; i++){
        pthread_create(&threads[i], 0, secao_critica, (void*)i);
    }

    for (long i = 0; i < QTD; i++){
        pthread_join(threads[i], NULL);
    }
    

    return 0;
}