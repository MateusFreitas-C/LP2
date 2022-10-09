#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(void) {
	pid_t clone_pid;
	int orig_pid;
	int return_val, exe_status;
	
	orig_pid = getpid();

	printf("Processo original em execucao...\n");
	printf("Pid do processo original: %d\n\n", orig_pid);

	sleep(5);

	clone_pid = fork();

	if (clone_pid >= 0) {
		//fork executado com sucesso
		if (clone_pid == 0) {
			//executado pelo processo clonado
			printf("[clone] Sou o processo clonado!\n");
			printf("[clone] Meu pid: %d\n", getpid());
			printf("[clone] Pid do processo original: %d\n", getppid());
			printf("[clone] Dormindo por 2 segundos...\n");
			sleep(2);
			printf("[clone] Insira um valor inteiro (0-255): ");
			scanf("%d", &return_val);
			printf("[clone] Finalizando...\n");
			exit(return_val);

		} else {
			//executado pelo processo original
			printf("[original] Sou o processo original!\n");
			printf("[original] Meu pid: %d\n", getpid());
			printf("[original] Pid do processo clonado: %d\n", clone_pid);
			printf("[original] Pid do processo criador: %d\n", getppid());
			printf("[original] Aguardando termino do clonado...\n");
			wait(&exe_status);
			printf("[original] Recebi do clonado: %d\n", WEXITSTATUS(exe_status));
			printf("[original] Finalizando...\n");
			exit(0);
		}

	} else {
		fprintf(stderr, "Erro ao invocar o fork()\n");
	}

	return 0;
}