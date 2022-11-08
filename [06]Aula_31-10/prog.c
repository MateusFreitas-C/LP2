#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

#define QTD 5

int number = 1;
int next = 1;
int turn[QTD];
int var_global = 0;

void* ticket(void* p){
    int count = 0;
    long index = (long)p;

    printf("Thread %ld iniciou\n", index);

    while(count <10){
        //protocolo de entrada

        /*
        <turn[index] = number; number ++>
        <await (turn[index] == next)
        */

       turn[index] = atomic_fetch_add(&number, 1);
       while(turn[index] != next);
        //protocolo de entrada

        //seção crítica
        printf("Thread %ld está na seção crítica! (%d)\n", index, turn[index]);
        var_global++;
        //seção crítica

        //protocolo de saída
        atomic_fetch_add(&next, 1);
        //protocolo de saída

        printf("Thread %ld está fora da seção crítica\n", index);
        count++;
    }
}



int main(){
    pthread_t threads[QTD];

    for (long i = 0; i < QTD; i++)
    {
        pthread_create(&threads[i], NULL, ticket, (void*)i);
    }

    for (long i = 0; i < QTD; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("Valor da variável global: %d\n", var_global);


    return 0;
}