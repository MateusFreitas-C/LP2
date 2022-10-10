/*Faça um programa em C/C++ que declare uma variável global do tipo double; 
faça o programa criar 10 threads e passe para essas threads um valor inteiro
(long) que represente um índice único (de 0 a 9). Faça cada uma das threads 
realizar uma função que verifique se seu identificador é par ou ímpar. Caso 
o identificador seja par, a função deve realizar um laço de repetição que 
faça 100.000 incrementos (+1) na variável global; caso o identificador seja
ímpar, a função deve realizar um laço que faça 50.000 decrementos (-1) na 
variável global. Após o término de todas as threads, exiba o valor final da
variável global na thread principal. Comente o código explicando o conceito 
de concorrência (e suas diferentes formas de realização nos atuais sistemas).
Fale também sobre consistência de memória (à luz da execução do código 
implementado), e o que poderia ser feito para sua garantia. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define QTD_PAR 100000
#define QTD_IMPAR 50000

long double x = 0;

void* funcao_thread(void* p) {
    long id = (long) p;
    int i = 0;
    long val = x;    
    if(id % 2 == 0 || id == 0){
        do
        {
            val++;                      //Aqui é executada a função da thread.
            i++;
        } while (i < QTD_PAR);
        
    }else{
        do
        {
            val--;
            i++;
        } while (i < QTD_IMPAR);
    }
    pthread_exit((void*)val);
}

int main(void) {
	pthread_t ths[10]; //Aqui é declarada uma variável para a criação da Threads
	void* ret[10]; //Variável que armazena o retorno das threads

	for (long i = 0; i < 10; i++){
    
        pthread_create(&ths[i], 0,
		funcao_thread,(void*)i); /*Aqui são criadas as threads e passado como parâmetro a variável do tipo p_thread 
        e a função que ela executará, bem como seu identificador*/
    }

    /*Thread é uma das formas de executar a programação concorrente no código.
    Quando falamos de concorrência estamos falando também de paralelismo e entrelaçamento, 
    uma interligação lógica entre o que está sendo feito, onde cada um vai ficar alocado.
    Além das threads existem também os processos, que na linguaguem C pode ser criado através
    da função fork(). Processo é um programa em execução e também uma identidade definida pelo SO para 
    agregrar todas as informações necessárias para a existência do processo.
	Contexto de Processo e de Threads: Código do Programa; dados armazenados na memória; pilha de execução;
    conteudo dos registradores; descritores dos arquivos abertos.
*/

		
	for (int i = 0; i < 10; i++) {
	       pthread_join(ths[i], &ret[i]);
           x += (long) ret[i];
	}

	printf("%.0Lf\n", x);
    /*Ao exibir o resultado é de se notar que ele nas primeiras execuções atinge o valor esperado de 250000
    mas ao repetir a execução ele muda os valores. Isso se deve ao fato da consitência de memória,
    que é você garantir que todos os entes de processamento do sistema tenham acesso a mesma 
    informação que está relacionada a mesma região de memória. Uma das formas de resolver seria utilizando o
    mutex_lock e mutex_unlock, protegendo uma seção crítica.
    */
	return 0;
}