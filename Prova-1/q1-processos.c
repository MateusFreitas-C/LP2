#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/signal.h>

void funcao_clone(int i) {
	printf("Novo Processo - %d\n", i);
}

int main(){
    int qtd_clones = 50;
    int* pid_clones;
    int ret_fork;
	double tempo = 0;
    int pid_original;

    pid_original = getpid();

	clock_t inicio = clock(); //inicio contagem

    pid_clones = (int*)malloc(sizeof(int)*qtd_clones);

    for (int i = 0; i < qtd_clones; i++) {
		if (getpid() == pid_original) {
			//processo original
			ret_fork = fork();  //criação de processo clone do original
			funcao_clone(i);
			if (getpid() == pid_original) { 
				//ainda processo original
				pid_clones[i] = ret_fork; //armazenando o retorno do fork do processo clone em um array
			}
		}
	}

	if (getpid() == pid_original) {
		//processo original
		printf("[original] finalizando clonados...\n");
		for (int i = 0; i < qtd_clones; i++) {
			kill(pid_clones[i], SIGHUP);  //finalizando os processos utilizando o pid armazenado no array.
		}
	}else{
		kill(getpid(), SIGHUP);
	}

	clock_t fim = clock(); // fim da contagem
	tempo += (double)(fim - inicio) / CLOCKS_PER_SEC; //obtenção do resultado em segundos.
	printf("O tempo gasto na criação dos processos foi de %f segundos.\n", tempo);

    return 0;
}