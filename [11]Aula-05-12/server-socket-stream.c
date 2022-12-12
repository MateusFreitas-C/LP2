#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

#define MY_PORT 9998

int main(){
    unsigned short port;
    char recv_buffer[32];
    int client_len;
    struct sockaddr_in server;
    struct sockaddr_in client;

    int server_sock;
    int client_sock;

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

    int l_ret = listen(server_sock, 10);

    if(l_ret == -1){
        printf("listen error (%d)\n", errno);
    }

    client_sock = accept(server_sock, (struct sockaddr *)&client, &client_len);

    if(client_sock == -1){
        printf("accept error (%d)\n", errno);
        exit(0);
    }

    char* str_addr = inet_ntoa(client.sin_addr);

    printf("Recebi uma conexão\n");
    printf("Endereço: %s\n", str_addr);

    char send_buf[14] = "Bem vindo!\n";

    int s_ret = send(client_sock, send_buf, sizeof(send_buf), 0);

    if(s_ret == -1){
        printf("send error\n");
    }

    int r_ret = recv(client_sock, recv_buffer, sizeof(recv_buffer), 0);

    printf("Recebi do cliente: %s\n", recv_buffer);

    close(client_sock);
    close(server_sock);

    return 0;
}