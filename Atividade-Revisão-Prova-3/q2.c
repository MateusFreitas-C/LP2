/*
Faça um programa em C/C++ que contenha um monitor que controle o acesso de escrita
e leitura de um arquivo binário, que armazena itens de acordo com a struct descrita abaixo: 

typedef struct {
	int operacao;
	char codigo;
	float valor;
} operacao_bancaria;

Faça a função principal instanciar este monitor e crie 5 threads para que estas
façam operações de escria e leitura no arquivo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define QTD_THREADS 5

typedef struct {
	int operacao;
	char codigo;
	float valor;
} operacao_bancaria;

typedef struct{
    char* filename;
    FILE* fd;
    pthread_mutex_t mutex;
}monitor;

monitor m;

void* file_write(void* p){
    
    operacao_bancaria *ob = (operacao_bancaria*)p;

    pthread_mutex_lock(&m.mutex);

    fwrite(ob, sizeof(operacao_bancaria), 1, m.fd);
    pthread_mutex_unlock(&m.mutex);

    printf("Operação = %d\n", ob->operacao);
    printf("Codigo = %d\n", ob->codigo);
    printf("Valor = %f\n", ob->valor);
}

void* file_read(void* p){


}

int main(){
    operacao_bancaria obs[QTD_THREADS];
    pthread_t threads[QTD_THREADS];
    time_t t;

    srand(time(&t));

    m.filename = (char*) malloc(sizeof(char)*20);
    strcpy(m.filename, "arquivo.bin");
    m.fd = fopen(m.filename, "w");
    pthread_mutex_init(&m.mutex, NULL);

    for(int i = 0; i< QTD_THREADS; i++){
        obs[i].operacao = rand() % 5;
        obs[i].codigo = 65 + (rand() % 5);
        obs[i].valor = rand() % 1000;
    }

    for(int i = 0; i< QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, file_write, (void*) &obs[i]);
    }

    for(int i = 0; i< QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    fclose(m.fd);

    return 0;
}