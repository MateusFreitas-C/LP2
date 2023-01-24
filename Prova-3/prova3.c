/*
(Questão 1 - 8 pontos) Faça um programa em C/C++ que implemente um servidor
que atenda os requisitos listados a seguir.
 
	1) Receba até 10 conexões simultâneas de clientes na porta 9622 TCP;
 
	2) Cada cliente deve enviar uma string ao se conectar ao servidor: 
	"START" para que a comunicação seja iniciada. Caso o cliente envie
	outra string, a conexão com ele deverá ser finalizada.
 
	3) O servidor tratará strings enviadas pelos clientes como comandos, e 
	retornára o resultado como descrito a seguir. Os seguintes comandos
	devem ser suportados: 
 
		a) CRTIME\n fará o servidor retornar a data e hora corrente;
 
		b) RNDNUM\n o servidor irá gerar e retornar um valor aleatório 
		entre 0 e 100;
 
		c) FINISH\n fará o servidor finalizar a conexão com o cliente;
 
		d) SHTDWN\n envia mensagem para todos os clientes informando 
		que o servidor será finalizado; finaliza o processo do servidor;
 
		e) Comandos não reconhecidos deverão fazer o servidor retornar
		"ERROR" para os clientes.

(Questão 2 - 4 pontos) Faça um monitor que controle o acesso de escrita e 
leitura em um arquivo de texto; modifique o código da questão anterior 
para que todos os comandos enviados pelos clientes sejam registrados em um
arquivo de texto cujo acesso é controlado pelo novo monitor implementado.
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

/*
Protocolo TCP X UDP

    TCP solicita conexão, que pode ser aceita ou não.
    As duas partes da conexão verifica se está ativa o tempo todo, e as duas sabem quando uma conexão acaba.
    Orientada a conexão (Garante ordem de entrega, e a própria entrega)

    UDP envia, mas não garante que chega o pacote.
    Não existe solicitação de conexão, não tem como saber se a informação chegou ou se o precesso está ativo.
    Não garante ordem de entrega, nem a entrega.

Modelo Cliente/Servidor

    Cliente: Processo ou aplicação que requisita um serviço oferecido por um servidor.
    Servidor: Processo ou aplicação que aceita as requisições dos clientes e realiza o processamento delas.

Stream Socket x Datagram Socket
    Stream Socket:
        -Mantém um socket único para cada cliente: canal dedicado
        -Trata a comunicação como um fluxo contínuo de bytes entre cliente e servidor
        -Utiliza o protocolo TCP

    Datagram Socket:
        -Servidor pode usar um mesmo socket para se comunicar com diferentes clientes: canal compartilhado
        -As mensagens são enviadas isoladamente
        -Utiliza o protocolo UDP 
*/

typedef struct{
    char* filename;
    FILE* fd;
    pthread_mutex_t mutex;
}monitor;
//Struct do monitor necessária para a questão 2
monitor m;

#define MY_PORT 9622 //Porta conectada
#define QTD_CLIENTS 10 //QTD DE CLIENTES
#define QTD_CLIENTS_SHUT 2 //THREAD AUXILIAR PARA SHUTDOWN

pthread_barrier_t barreira; //BARREIRA PARA TRATAR SHUTDOWN

int client_sock[QTD_CLIENTS];

void shutdown_handler(void* p){  //FUNÇÃO DA THREAD QUE TRATA O SHUTDOWN
    pthread_barrier_wait(&barreira);
    printf("Shutdown recebida\n");
    exit(0);
}

void* client_handler(void* p){ //FUNÇÃO QUE TRATA A CONEXÃO
    int _client_sock = (int)p;
    char start_buffer[8];
    char init_buf[10] = "\nSTART\n";

    //Função que envia mensagens para o cliente
    int s_ret = send(_client_sock, init_buf, sizeof(init_buf), 0); 

    while(1){
        bzero(start_buffer, 8);
        //Função que recebe mensagem enviada pelo cliente
        int r_ret = recv(_client_sock, start_buffer, sizeof(start_buffer), 0);

        //Monitor controlando acesso de escrita ao arquivo.
        pthread_mutex_lock(&m.mutex);
        fwrite(start_buffer, sizeof(start_buffer), 1, m.fd);
        pthread_mutex_unlock(&m.mutex);

        if(strncmp(start_buffer, "START\n", 5) != 0){
            //Função que finaliza a conexão do cliente
            close(_client_sock);
            pthread_exit(0);

        }else{
            while(1){
                char recv_buffer[10];
                
                bzero(recv_buffer, 32);
                int r_ret = recv(_client_sock, recv_buffer, sizeof(recv_buffer), 0);
                printf("\nRecebi do cliente(%d): %s\n", _client_sock, recv_buffer);
                
                //Monitor controlando o acesso de escrita ao arquivo.
                pthread_mutex_lock(&m.mutex);
                fwrite(recv_buffer, sizeof(recv_buffer), 1, m.fd);
                pthread_mutex_unlock(&m.mutex);

                if(strncmp(recv_buffer, "CRTIME\n", 6) == 0){
                    struct tm *t;
                    time_t seconds;
                    size_t n = 40;
                    time(&seconds);

                    printf("%s\n", ctime(&seconds));
                    char* str = ctime(&seconds);

                    int s_ret = send(_client_sock, str, n, 0);
                
                }else if(strncmp(recv_buffer, "RNDNUM\n", 6) == 0){
                    int num = rand() % 100;
                    char str[5];
                    printf("%d\n", num);
                    sprintf(str, "%d", num);
                    strcat(str, "\n");
                    int s_ret = send(_client_sock, str, sizeof(str), 0);

                }else if(strncmp(recv_buffer, "FINISH\n",6) == 0){
                    close(_client_sock);
                    pthread_exit(0);

                }else if(strncmp(recv_buffer, "SHTDWN\n",6) == 0){
                    
                    for (int i = 0; i < QTD_CLIENTS; i++){
                        int s_ret = send(client_sock[i], "FINALIZANDO SERVIDOR\n", sizeof("FINALIZANDO SERVIDOR\n"), 0);
                    }
                    pthread_barrier_wait(&barreira);
                    pthread_exit(0);    
                    exit(0);  
                }else{
                    int s_ret = send(_client_sock, "ERROR\n", sizeof("ERROR\n"), 0); 
                }
            }
        }        
    }
}

int main(){
    unsigned short port;
    int client_len;
    struct sockaddr_in server;
    struct sockaddr_in client[QTD_CLIENTS];

    m.filename = (char*) malloc(sizeof(char)*20);
    strcpy(m.filename, "arquivo.txt");
    m.fd = fopen(m.filename, "w");
    pthread_mutex_init(&m.mutex, NULL);

    int server_sock;
    int client_counter = 0;

    int init_ret = pthread_barrier_init(&barreira, NULL, QTD_CLIENTS_SHUT);

    if(init_ret != 0){
        printf("[main] Erro ao iniciar barreira (%d)\n", init_ret);
        exit(0);
    }

    pthread_t client_threads[QTD_CLIENTS];
    pthread_t shut_thread;

    printf("Servidor aguardando na porta %d\n", MY_PORT);

    pthread_create(&shut_thread, 0, shutdown_handler, 0);

    //Criação do socket TCP, caso fosse UDP seria SOCK_DGRAM
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sock == -1){
        printf("Socket error (%d)\n", errno);
        exit (0);
    }
    
    /*
    Struct definindo as informações do servidor como um servidor TCP na porta escolhida.
    */
    server.sin_family = AF_INET;
    server.sin_port = htons(MY_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    //Função bind associa o socket criado a porta local do sistema operacional
    int b_ret = bind(server_sock, (struct sockaddr *)&server, sizeof(server));

    if(b_ret == -1){
        printf("bind error (%d)\n", errno);
        exit (0);
    }

    //Função que habilita o servidor para receber informações do cliente, não necessária em servidores UDP
    int l_ret = listen(server_sock, QTD_CLIENTS);

    if(l_ret == -1){
        printf("listen error (%d)\n", errno);
    }

    while(1){
        //Função que aceita as conexões solicitadas ao servidor pelo cliente
        client_sock[client_counter] = accept(server_sock, (struct sockaddr *)&client[client_counter], &client_len);

        if(client_sock[client_counter] == -1){
            printf("accept error (%d)\n", errno);
            exit(0);
        }
        //criação das threads para tratar dos clientes
        pthread_create(&client_threads[client_counter], 0, client_handler, (void*)client_sock[client_counter]);

        client_counter++;
    }
    //Fechando o servidor
    close(server_sock);

    return 0;
}