#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {

	int pid_o = getpid();
	printf("START\n");
	sleep(20);
	int rf = fork();
	printf("fork()!\n");
	sleep(20);
	if (getpid() == pid_o) {
		fork();
	}
	printf("FINISH\n");
	sleep(20);

	return 0;

}