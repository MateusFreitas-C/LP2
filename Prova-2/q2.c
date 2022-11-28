/*(Questão 2 - 5 pontos) Faça um programa em C/C++ que controle o acesso de 
threads a duas seções críticas diferentes. O programa deverá ter 10 threads,
e duas variáveis globais (x e y) de tipo int, iniciadas com zero. As threads
modificarão em loop e em sequência as duas variáveis, porém a ordem de 
modificação dependerá do valor do índice que receberão no momento de sua 
criação: caso o índice seja par, a thread incrementa o valor da variável x com 
um valor aleatório entre 5 e 100 e dorme 200ms, em seguida incrementa o valor 
da variável y com um valor aleatório entre 1 e 50 e dorme 100ms; caso o índice 
seja ímpar, a thread incrementa o valor da variável x com um valor aleatório 
entre 1 e 30 e dorme 100ms, em seguida incrementa o valor da variável y com 
um valor aleatório entre 15 e 95 e dorme 250ms.
 
Use semáforos (biblioteca semaphore.h) para controlar o acesso às seções 
críticas, (trate o acesso à cada uma das variáveis como uma seção crítica
diferente) e garanta que todas as  threads  incrementem as variáveis globais
dentro de um ciclo de iterações. O programa deverá ser finalizado quando a 
soma dos valores de x e y for maior que 2000. Comente o código explicando 
o que é uma seção crítica, e como foi feito o controle de acesso às duas 
seções críticas do problema usando semáforos.*/

//SC = Seção Crítica

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define QTD 10

/*
    Com semáforos é possível intercalar o acesso das threads a seção crítica. Seção crítica é um trecho de código que manipula um recurso compartilhado.
    Para solucinar o problema da seção crítica são respeitadas 4 propriedades:
        Exclusão Mútua - garante que apenas 1 processo ou thread esteja executando a seção crítica
            
        Ausência de Deadlock - Se dois ou mais processos estiverem tentando acessar a seção crítica, no mínimo um processo poderá acessar a seção crítica
		
		Ausência de atraso desnecessário - Se a seção crítica está disponível, alguém tem que estar lá.
		
		Entrada Eventual - Garante que todas as threads eventualmente acessem a seção crítica

    No programa são duas variáveis e enquanto uma é incrementada a outra espera até dar o sem_post, para assim as threads alternarem o acesso a seção crítica
*/

sem_t mutex_x, mutex_y;
int x = 0, y = 0;

void* secao_critica_x(void* p){
    long index = (long)p;

    printf("[%ld] iniciou\n", index);
    while (x + y <= 2000){
        int incremento;
        
        if(index % 2 == 0){
            incremento = rand() % 100 - 5;
            sem_wait(&mutex_x);
		
            printf("[%ld] seção critica\n", index);

            if ((x + y) <= 2000){
                x += incremento;
            } 
            printf("X + Y = %d\n", x + y);
            
            usleep(200);

            sem_post(&mutex_y);
        }else{
            incremento = rand() % 30 - 1;
            sem_wait(&mutex_x);
		
            printf("[%ld] seção critica\n", index);

        if ((x + y) <= 2000){
                x += incremento;
            } 
            printf("X + Y = %d\n", x + y);
            
            usleep(100);

            sem_post(&mutex_y);
        }
    }
}

void* secao_critica_y(void* p){
    long index = (long)p;

    printf("[%ld] iniciou\n", index);
    while (x + y <= 2000){
        int incremento;

        if(index % 2 == 0){
            incremento = rand() % 50 -1;
            sem_wait(&mutex_y);

            printf("[%ld]seção crítica\n", index);

            if((x + y) <= 2000){
                y += incremento;
            }

            printf("X + Y = %d\n", x + y);
            usleep(100);

            sem_post(&mutex_x);

        }else{
            incremento = rand() % 95 - 15;
            sem_wait(&mutex_y);
            printf("[%ld]seção crítica\n", index);

            if((x + y) <= 2000){
                y += incremento;
            }
            printf("X + Y = %d\n", x + y);
            usleep(250);
            sem_post(&mutex_x);
        }
    }
}


int main(){
    pthread_t threads[QTD];
    time_t t;

    srand(time(&t));

    sem_init(&mutex_x, 0, 1);
    sem_init(&mutex_y, 0, 1);

   for (long i = 0; i < QTD; i++) {
		if(i % 2 == 0){
            pthread_create(&threads[i], NULL, secao_critica_x, (void*)i);
        }else{
            pthread_create(&threads[i], NULL, secao_critica_y, (void*)i);
        }
	}

    for (long i = 0; i < QTD; i++){
        pthread_join(threads[i], NULL);
    }
    
    printf("X + Y = %d\n", x + y);

    return 0;
}