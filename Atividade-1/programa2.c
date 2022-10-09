/* Ler um valor inteiro e exibe essa quantidade de linhas com uma string de prefixo*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    char str[30];

    fgets(str, 30, stdin);

    printf("Aula de LP2 com Bidu: %s", str);

   return 0; 
}