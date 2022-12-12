/*
Faça um programa em C/C++ que crie um servidor TCP que aguarde conexões na porta
9000. Este servidor deverá ser capaz de receber e tratar conexões de até 10 
clientes simultâneos. Cada nova conexão deverá receber uma string de início de 
conexão (ex. “START”). Faça o programa gerenciar um buffer compartilhado composto
por até 1024 caracteres. Este buffer deverá ser modificado concatenando cada nova
string recebida de cada um dos clientes. Sempre que receber uma nova string de um
dos clientes, o servidor modifica o buffer e o envia para todos os cliente.

Exemplo de execução:
	3 clientes se conectam;
	servidor envia “START” para os 3 clientes;
	cliente 1 envia a string “ABC” para o servidor;
	servidor envia buffer para o cliente 1: “ABC”;
	cliente 2 envia a string “XYZ” para o servidor;
	servidor envia buffer para cliente 2: “ABC\nXYZ”
	cliente 3 envia a string “IJK” para o servidor;
	servidor envia buffer para cliente 3: “ABC\nXYZ\nIJK”;

Se 4 dos clientes enviarem a string “SHUTDOWN” o servidor deverá ser 
finalizado. Utilize uma barreira para implementar esta funcionalidade.

O programa deverá utilizar a biblioteca POSIX Threads (PThreads) ou as threads
STD (C++11) e a biblioteca sockets.h (ou winsock.h). A saída do seu programa 
deve ser bem planejada, de forma a mostrar o que está acontecendo em cada 
momento relevante. Para testes do servidor, usar netcat (nc) ou telnet para 
o lado do cliente.
*/  

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>

#define MY_PORT 9000
#define QTD_CLIENTS 10

char final_buffer[1032] = "";
pthread_barrier_t barreira;
int counter = 0;

void* client_handler(void* p){
    int client_sock = (int)p;
    char recv_buffer[32];
    char init_buf[10] = "\nSTART\n";
    
    bzero(recv_buffer, 32);

    int s_ret = send(client_sock, init_buf, sizeof(init_buf), 0);

    while(counter < 4){
        int r_ret = recv(client_sock, recv_buffer, sizeof(recv_buffer), 0);

        printf("\nRecebi do cliente(%d): %s\n", client_sock, recv_buffer);

        if(strncmp("SHUTDOWN\n", recv_buffer, sizeof(recv_buffer)) == 0){
            counter++;
            printf("Contador = %d\n", counter);
        }

        strcat(final_buffer, recv_buffer);

        int s_ret = send(client_sock, final_buffer, sizeof(final_buffer), 0);  
    }
}

int main(){
    unsigned short port;
    int client_len;
    struct sockaddr_in server;
    struct sockaddr_in client[QTD_CLIENTS];

    int server_sock;
    int client_sock[QTD_CLIENTS];
    int client_counter = 0;

    int init_ret = pthread_barrier_init(&barreira, NULL, QTD_CLIENTS);

    if(init_ret != 0){
        printf("[main] Erro ao iniciar barreira (%d)\n", init_ret);
        exit(0);
    }

    pthread_t client_threads[QTD_CLIENTS];

    printf("Servidor aguardando na porta %d\n", MY_PORT);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sock == -1){
        printf("Socket error (%d)\n", errno);
        exit (0);
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(MY_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    int b_ret = bind(server_sock, (struct sockaddr *)&server, sizeof(server));

    if(b_ret == -1){
        printf("bind error (%d)\n", errno);
        exit (0);
    }

    int l_ret = listen(server_sock, QTD_CLIENTS);

    if(l_ret == -1){
        printf("listen error (%d)\n", errno);
    }

    while(1){
        client_sock[client_counter] = accept(server_sock, (struct sockaddr *)&client[client_counter], &client_len);

        if(client_sock[client_counter] == -1){
            printf("accept error (%d)\n", errno);
            exit(0);
        }

        pthread_create(&client_threads[client_counter], 0, client_handler, (void*)client_sock[client_counter]);

        client_counter++;
    }

    close(server_sock);

    return 0;
}