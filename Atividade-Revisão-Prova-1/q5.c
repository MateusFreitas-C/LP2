#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define MODO_THREAD 0
#define MODO_PROCESSO 1

#define SALTO_MAX_LEBRE 20

int modo;
int tamanho_pista;
int qtd_lebre;

void* lebre(void* p) { 
	long id = (long) p;
	int soma_salto = 0;
	int salto_max = rand() % SALTO_MAX_LEBRE;
	printf("[lebre-%ld] iniciou\n", id);
	while (soma_salto < tamanho_pista) {
		int salto = (rand() % salto_max)+1;
		soma_salto += salto;
		printf("[lebre-%ld] saltou %d cm (%d)\n",
				id, salto, soma_salto);
	}
	usleep(10);
	printf("[lebre-%ld] vencedora!\n", id);
	fflush(stdout);
	exit(0);
}

int main(int argc, char* argv[]) {
	int main_pid;
	main_pid = getpid();

        if (argc < 4) {
		fprintf(stderr, "Poucos argumentos\n");
	} else {
		if (argv[1][0] == 't') {
			modo = MODO_THREAD;
		} else if (argv[1][0] == 'p') {
			modo = MODO_PROCESSO;
		} else {
			printf("Modo nao reconhecido!\n");
			exit(-1);
		}
		qtd_lebre = atoi(argv[2]);
		tamanho_pista = atoi(argv[3]);
		printf("Modo: %d\n", modo);
	        printf("Quantidade: %d\n", qtd_lebre);
		printf("Tamanho: %d cm\n", tamanho_pista);	
	}
	if (modo == MODO_THREAD) {
		pthread_t *threads;
		threads = (pthread_t*)
			malloc(
			sizeof(pthread_t)*qtd_lebre);

		//corrida thread
		for (long i = 0; i < qtd_lebre; i++) {
			pthread_create(&threads[i],
					0, lebre, 
					(void*) i);
		}
		for (int i = 0; i < qtd_lebre; i++) {
			pthread_join(threads[i], NULL);
		}
	} else {
		//corrida processo
		pid_t* procs;
		procs = (pid_t*)malloc(
				sizeof(pid_t)*qtd_lebre);
		int f_res;

		for (long i = 0; i < qtd_lebre; i++) {
			if (main_pid == getpid()) {
				f_res = fork();
				if (main_pid == getpid()) {
					procs[i] = f_res;
				}
			} else {
				lebre((void*)i);
			}
		}
		if (main_pid == getpid()) {
		    wait(NULL);
		    printf("[main] chegada detectada!\n");
		    for (int i = 0; i < qtd_lebre; i++) {
			    kill(procs[i], SIGHUP);
		    }
		    exit(0);
		}
	}


	return 0;
}
