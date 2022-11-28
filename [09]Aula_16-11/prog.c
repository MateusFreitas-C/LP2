#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>

#define QTD_PROD 1
#define QTD_CONS 4

int buf;
sem_t buf_empty, buf_full;

void* producer(void* p){
    long index = (long)p;

    while(1){
        sem_wait(&buf_empty);
        buf = rand() % 100;
        printf(" [prod %ld] gerou %d\n", index, buf);
        sleep(1);
        sem_post(&buf_full);
    }
}

void* consumer(void* p){
    long index = (long)p;
    int result;

    while (1){
        sem_wait(&buf_full);
        result = buf;
        printf("[cons %ld] consumiu %d\n", index, result);
        sleep(1);
        sem_post(&buf_empty);
    }
    
}

int main(){
    time_t t;
    srand(time(&t));
    pthread_t prod[QTD_PROD];
    pthread_t cons[QTD_CONS];
    
    sem_init(&buf_empty, 0, 1);
    sem_init(&buf_full, 0, 0);

    for(long i = 0; i < QTD_PROD; i++){
        pthread_create(&prod[i], 0, producer, (void*) i);
    }

    for(long i = 0; i < QTD_CONS; i++){
        pthread_create(&cons[i], 0, consumer, (void*) i);
    }

    sleep(60);

    return 0;
}
