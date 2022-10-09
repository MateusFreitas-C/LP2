#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	char* my_args[2] = {"./pa-test", "STRING_TEST"};
	printf("Sou o programa execv\n");
	long x = 0;
	while (x < 9999999999) {
		x++;
	}
	int ret_fork = fork();
	if (ret_fork == 0) {
		x = 0;
		while (x < 9999999999) x++;
		//processo clonado
		execv("./pa-test", my_args);
	}
	printf("Sou o processo original\n");
	printf("Sou o programa exec-test\n");

	x = 0;

	while (x < 9999999999) { x++; }
	return 0;

}