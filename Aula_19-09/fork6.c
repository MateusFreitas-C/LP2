#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

int main(void) {
	pid_t clone_pid[2];
	int orig_pid;
	int return_val, exe_status;
	time_t t;

	orig_pid = getpid();
//	srand(time(&t));

	printf("Processo original em execucao...\n");

	clone_pid[0] = fork();

	if (getpid() == orig_pid) {
		int temp1, temp2;
		clone_pid[1] = fork();
		if (getpid() == orig_pid) { 
			waitpid(clone_pid[0], &temp1, 0);
			printf("Recebi de %d o valor %d\n", 
					clone_pid[0],
					WEXITSTATUS(temp1));
			waitpid(clone_pid[1], &temp2, 0);
			printf("Recebi de %d o valor %d\n",
					clone_pid[1],
					WEXITSTATUS(temp2));
		} 
	} 
	
	if (getpid() != orig_pid) {
		srand(getpid());
		//clonados
		sleep(3);
		int r = rand() % 256;
		printf("Sou %d e saindo com valor %d\n", getpid(), r);
		exit(r);
	}

	return 0;
}
