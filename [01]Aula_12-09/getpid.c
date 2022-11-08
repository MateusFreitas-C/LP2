#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){

    printf("Antes %d\n", getpid());

    sleep(10);

    int rf = fork();

    printf("Depois %d %d\n", rf, getpid());

    if(rf == 0){
        printf("Processo clonado!. %d \n", getpid());
    
    }else{
        printf("Processo Original! %d \n", getpid());

    }

    sleep(20); //Sleep para olhar os processos no terminal

    return 0;
}