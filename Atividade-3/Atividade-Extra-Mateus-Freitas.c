/*
1) Faça um programa em C/C++ que implemente o algoritmo do ticket, onde 10 
threads se alternam para acessar uma seção crítica. A função a ser executada
na seção crítica deve exibir seu índice, a data e hora do sistema e incrementar
um contador armazenado em uma variável global. Explique em comentários porque 
esta solução atende às 4 propriedades de uma boa solução discutidas em sala de 
aula. 
*/


/*
    O Algorítmo do Ticket baseia-se na distribuição de fichas que indicam a ordem de chegada. Como um caixa de banco que distribui
    fichas para os clientes, ele atende o problema da exclusão mútua, pois como é possível ver pelos printf's do programa, apenas uma
    thread pode estar executando a seção crítica em um instante de tempo. Com isso, garante o princípio do Livelock, onde se dois processos
    estiverem tentando executar a região crítica, apenas um obterá sucesso. Também é possível observar pelos printf's que as outras thread não
    congelam ao entrar na seção crítica, e sim continuam executando suas seções não críticas, não impedindo a thread que quer entrar na seção crítica.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define QTD 10

int number = 1;
int next = 1;
int turn[QTD];
int var_global = 0;

void hora(){
    struct tm *t;
    time_t seconds;

    time(&seconds);
    t = localtime(&seconds);
    printf("%s\n", ctime(&seconds));
}

void* ticket(void* p){
    long index = (long)p;

    printf("Thread %ld iniciou\n", index);
    sleep(1);

    while(1){
        //protocolo de entrada
       turn[index] = atomic_fetch_add(&number, 1);
       while(turn[index] != next);
        //protocolo de entrada

        //seção crítica
        printf("Thread %ld está na seção crítica!\n", index);
        hora();
        var_global++;
        //seção crítica

        //protocolo de saída
        atomic_fetch_add(&next, 1);
        //protocolo de saída
        printf("Thread %ld saiu da seção crítica!\n", index);
        sleep(1);
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