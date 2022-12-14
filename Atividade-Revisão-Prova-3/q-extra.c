#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

#define MY_PORT 9000

int main(){
    unsigned short port;
    char recv_buffer[1024];
    char client_msg[32];
    int client_len;
    struct sockaddr_in server_info;
    struct sockaddr_in client;

    int client_sock;

    struct hostent *hp;

    hp = gethostbyname("localhost");

    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (client_sock == -1){
        printf("Socket error (%d)\n", errno);
        exit (0);
    }

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(MY_PORT);
    server_info.sin_addr.s_addr = *((unsigned long *)hp->h_addr);

    int ret_con = connect(client_sock, (struct sockaddr *)&server_info, sizeof(server_info));

    if(ret_con == -1){
        printf("Connection error (%d)\n", errno);
        exit(0);
    }

    printf("Conexão realizada\n");

    while (strcmp(client_msg, "EXIT") !=0){

        bzero(recv_buffer, 1024);

        int r_ret = recv(client_sock, recv_buffer, sizeof(recv_buffer), 0);

        printf("Recebi do Servidor: %s", recv_buffer);

        fgets(client_msg, 32, stdin);

        int s_ret = send(client_sock, client_msg, strlen(client_msg),0);
    }
    
    close(client_sock);

    return 0;
}