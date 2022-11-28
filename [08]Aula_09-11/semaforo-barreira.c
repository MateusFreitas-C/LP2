#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define QTD 4

sem_t mutex[QTD];

void tarefa(long i){
    printf("[%ld] tarefa()\n", i);
}

void* barreira(void* p){
    long index = (long)p;

    printf("[%ld] iniciou\n", index);
    while (1){
        tarefa(index);
        printf("[%ld] chegou na barreira\n", index);
        sem_post(&mutex[index]);
        for (long i = 0; i < QTD; i++)
        {
            if(i != index){
                sem_wait(&mutex[i]);
            }
        }
        printf("[%ld] Passou da barreira\n", index);
        sleep(1);
    }
}

int main(){
    pthread_t threads[QTD];


    for (long i = 0; i < QTD; i++){

        sem_init(&mutex[i], 0, 0);
    }
    

    for (long i = 0; i < QTD; i++){
        pthread_create(&threads[i], 0, barreira, (void*)i);
    }

    for (long i = 0; i < QTD; i++){
        pthread_join(threads[i], NULL);
    }
    

    return 0;
}