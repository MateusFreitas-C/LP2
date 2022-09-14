#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){

    printf("Antes\n");

    fork();

    printf("Depois\n");

    return 0;
}