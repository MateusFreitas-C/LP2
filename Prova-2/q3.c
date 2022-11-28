/*(Questão 3 - 5 pontos) Faça um programa em C/C++ que simule o controle de
temperatura de um motor. O programa deverá ter uma variável global do tipo
float chamada temperatura (que armazenará a temperatura em Celsius do motor,
com valor inicial 600 C) e duas threads além da main, realizando funções 
diferentes. A primeira thread  realizará   um   incremento   na   temperatura
com um valor aleatório entre -10 e 90 e depois dormirá 200ms. A segunda 
thread deverá conter um mecanismo que detecte o quanto antes quando o valor da 
temperatura ultrapassar 1000C - quando isso acontecer deve-se dormir 300ms e o
valor da variável temperatura passa a ser 600 novamente.
 
O programa deverá ser finalizado depois de 3 reduções de temperatura do motor.
Comente o código explicando o mecanismo utilizado na segunda thread e qual 
seria a alternativa se não houvesse o requisito de detecção da temperatura de
forma mais imediata possível. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

#define QTD 2

float temperatura = 600;
int count = 0;

pthread_barrier_t barreira;

/*
O programa foi feito utilizando barreira. Uma barreira é um ponto de sincronização que todos os processos precisam alcançar antes de prosseguir seu processamento.
A alternativa seria na mesma thread, após o incremento verifica se a temperatura superou os 1000ºC e diminuir.
*/


void* decrementar_temperatura(void* p){
    long index = (long) p;
    printf("[%ld] iniciou\n", index);
    while (count < 3){
        
        if (temperatura >= 1000)
        {
            printf("[%ld] Passou dos 1000\n", index);
            usleep(300);
            temperatura = 600;
            count++;
        }
        pthread_barrier_wait(&barreira);
    }
}

void* incrementar_temperatura(void* p){
    long index = (long) p;
    printf("[%ld] iniciou\n", index);
    while (count < 3){
        int incremento;
        incremento = -10 + (rand() % 90);
        temperatura +=incremento;
        pthread_barrier_wait(&barreira);
        printf("[%ld] Passou da barreira: Temperatura %f\n", index, temperatura);
        usleep(200);
    }
}

int main(){
    pthread_t threads_1;
    pthread_t threads_2;

    time_t t;
    srand(time(&t));

    int init_ret = pthread_barrier_init(&barreira, NULL, QTD);

    if(init_ret != 0){
        printf("[main] Erro ao iniciar barreira (%d)\n", init_ret);
        exit(0);
    }

    pthread_create(&threads_1, NULL, incrementar_temperatura,(void*)0);
    pthread_create(&threads_2, NULL, decrementar_temperatura,(void*)1);

    pthread_join(threads_1, NULL);
    pthread_join(threads_2, NULL);

    return 0;
}

