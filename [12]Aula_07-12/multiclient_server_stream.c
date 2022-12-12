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

#define MY_PORT 9998
#define QTD_CLIENTS 5

void* client_handler(void* p){
    int client_sock = (int)p;
    char recv_buffer[32];

    bzero(recv_buffer, 32);

    while(1){
        int r_ret = recv(client_sock, recv_buffer, sizeof(recv_buffer), 0);

        printf("Recebi do cliente(%d): %s\n", client_sock, recv_buffer);

        printf("Ecoando mensagem...\n");

        int s_ret = send(client_sock, recv_buffer, sizeof(recv_buffer), 0);
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