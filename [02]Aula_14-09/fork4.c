#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main(void) {

	int pid_o = getpid();
	printf("START\n");
	sleep(20);

	int rf = fork();
	printf("fork()!\n");

	if (getpid() == pid_o) {
		sleep(10);
		printf("finalizando clonado: %d\n", rf);
		kill(rf, -9);
	}

	sleep(20);

	return 0;

}