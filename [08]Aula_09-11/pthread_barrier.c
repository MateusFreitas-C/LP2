#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD 5

pthread_barrier_t barreira;

void tarefa(long index){
    printf("[%ld] tarefa()\n", index);
    sleep((int)index + 1);

}

void* tarefa_thread(void* p){
    long index = (long) p;
    printf("[%ld] iniciou\n", index);

    while (1){
        tarefa(index);
        printf("[%ld] Antes da barreira\n", index);
        pthread_barrier_wait(&barreira);
        printf("[%ld] Passou da barreira\n", index);
    }
}

int main(){
    pthread_t threads[QTD];

    int init_ret = pthread_barrier_init(&barreira, NULL, QTD);


    if(init_ret != 0){
        printf("[main] Erro ao iniciar barreira (%d)\n", init_ret);
        exit(0);
    }


    for (long i = 0; i < QTD; i++)
    {
        pthread_create(&threads[i], NULL, tarefa_thread,(void*)i);
    }

    for (long i = 0; i < QTD; i++){

        pthread_join(threads[i], NULL);
    }

    return 0;
}