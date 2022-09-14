#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){

    printf("Start\n");
    sleep(5);
    int rf = fork();

    printf("fork\n");
    sleep(5);
    if(rf == 0){
        fork();
    }
    sleep(5);
    printf("Depois\n");

    return 0;
}