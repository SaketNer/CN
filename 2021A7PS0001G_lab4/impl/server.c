/*
example port details
host = '127.0.0.1'
port = 2021
*/

#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 

// File Pointer declared 
//FILE* ptr; 

char msg[1024];
char reply[1024];

#define no_of_clients 3
//int no_of_clients = 3;
int client_id[no_of_clients];
char name[no_of_clients][1024];
int clientno = no_of_clients;


// create connction
int create_connection(char* addr, int port) {
	int server_sockfd;
	struct sockaddr_in server_addrinfo;
	
	//socket
	if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("server: socket");
        exit(1);
    }
	
	int yes = 1;
	if(setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror("setsockopt");
        close(server_sockfd);
        exit(1);
    }

	server_addrinfo.sin_family = AF_INET;
    server_addrinfo.sin_port = htons(port);
    server_addrinfo.sin_addr.s_addr = htonl(INADDR_ANY);

	if (inet_pton(AF_INET, addr, &server_addrinfo.sin_addr) <= 0) {
        //printf("\nInvalid address/ Address not supported \n");
        close(server_sockfd);
        exit(1);
    }

	//  BIND
	if(bind(server_sockfd, (struct sockaddr*) &server_addrinfo, sizeof(server_addrinfo)) == -1){
        perror("server: bind");
        close(server_sockfd);
        exit(1);
    }

	// LISTEN 
	if(listen(server_sockfd, 0) == -1){ // 0 indicates the number of pending connections
        perror("listen");
        close(server_sockfd);
        exit(1);
    }
	return server_sockfd;

}

// Accept incoming connections
int client_connect(int socket_id) {
	//ACCEPT
	struct sockaddr_in client_addrinfo;
	socklen_t sin_size = sizeof(client_addrinfo);
	int new_server_sockfd = accept(socket_id, (struct sockaddr*) &client_addrinfo, &sin_size);
    if(new_server_sockfd == -1){
        perror("accept");
        close(socket_id);
        exit(1);
    }

	char client_IP[INET6_ADDRSTRLEN];
    unsigned int client_port;

	//inet_ntop converts the networkaddresses from binary to text form
	//is not being used in the code
	if(inet_ntop(client_addrinfo.sin_family, &client_addrinfo.sin_addr, client_IP, sizeof(client_IP)) <= 0){
        //printf("\nAddress Conversion Error\n");
        close(socket_id);
        close(new_server_sockfd);
        exit(1);
    }
    //client_port = ntohs(client_addrinfo.sin_port);
    //printf("Server: Got connection from %s:%u\n", client_IP, client_port);

	return new_server_sockfd;

}

void get_name(){
    memset(msg, 0, 1024);
    memset(reply, 0, 1024);
    int recv_count;
    for(int i =0; i <no_of_clients;i++){
        strcpy(msg,"NAME\n");

        // 6. SEND
		int send_count;
		if((send_count = send(client_id[i], msg, strlen(msg), 0)) == -1){
			perror("send");
			close(client_id[i]);
			//close(new_server_sockfd);
			exit(1);
		}

        //RECEIVE
        if((recv_count = recv(client_id[i], reply, 1024, 0)) == -1){
            perror("recv");
            close(client_id[i]);
            //close(new_server_sockfd);
            exit(1);
        }
        strcpy(name[i],reply);
    }
	   
}

void server_logic(){
    while (clientno > 0 )
    {   //printf("Polling\n");
        for(int i =0; i < no_of_clients; i++){
            if(client_id[i] == -1){
                continue;
            }
            //fprintf( ptr,"polling %d\n",i);
            //fflush(ptr);
            memset(msg, 0, 1024);
            memset(reply, 0, 1024);

            strcpy(msg,"POLL\n");

            // 6. SEND
            int send_count;
            if((send_count = send(client_id[i], msg, strlen(msg), 0)) == -1){
                perror("send");
                close(client_id[i]);
                //close(new_server_sockfd);
                exit(1);
            }

            //RECEIVE
            int recv_count;
            if((recv_count = recv(client_id[i], reply, 1024, 0)) == -1){
                perror("recv");
                close(client_id[i]);
                //close(new_server_sockfd);
                exit(1);
            }
            if(strcmp(reply,"NOOP\n")==0||strcmp(reply,"NOOP")==0){

            }
            else if(strcmp(reply,"EXIT\n")==0||strcmp(reply,"EXIT")==0){
                //printf("Client %s is active\n",name[i]);
                clientno--;
                client_id[i] = -1;
            }
            else if(strcmp(reply,"LIST\n")==0||strcmp(reply,"LIST")==0){
                //printf("Client %s is active\n",name[i]);
                memset(msg, 0, 1024);
                for(int j = 0; j < no_of_clients; j++){
                    if(client_id[j] != -1){
                        //sprintf(msg, "%s%s:", msg, name[j]);
                        char temp[1024];
                        strcpy(temp, name[j]);
                        temp[strcspn(temp, "\n")] = 0;
                        strcat(msg,temp );
                        char str3[] = ":";
                        strcat(msg, str3);
                    }
                }
                msg[strlen(msg)-1] = '\0';
                if((send_count = send(client_id[i], msg, strlen(msg), 0)) == -1){
                     perror("send");
                    close(client_id[i]);
                    //close(new_server_sockfd);
                    exit(1);
                }
                //fprintf( ptr,"sending: %s\n",msg);
                //fflush(ptr);
            }

            else if(strncmp(reply,"MESG",4)==0){
                memset(msg, 0, 1024);
                //strcpy(msg, "INVALID CMD\n");
                char temp[1024];
                strcpy(temp,name[i]);
                temp[strcspn(temp, "\n")] = 0;
                strcat(msg,temp);
                char str3[] = ":";
                strcat(msg, str3);
                char* token = strtok(reply, ":");
                token = strtok(NULL, ":");
                strcat(msg,token);
                printf("%s",msg);
                //fprintf( ptr,"%s",msg);
                //fflush(ptr);
                fflush(stdout);
            }
            else{
                printf("INVALID CMD\n");
                //fprintf( ptr,"INVALID CMD\n");
                //fflush(ptr);
                fflush(stdout);
            }
            

        }


    }
    printf("SERVER TERMINATED: EXITING......");
    fflush(stdout);
    
}

int main(int argc, char *argv[])
{   
    if (argc != 3)
	{
		printf("Use 2 cli arguments\n");
		return -1;
	}
    //ptr = fopen("./Hello.txt", "w"); 
    
    
	
	// extract the address and port from the command line arguments
	// extract the address and port from the command line arguments
	char addr[INET6_ADDRSTRLEN];
	strcpy(addr, argv[1]);
	int port = atoi(argv[2]);

	int socket_id = create_connection(addr, port);
    for(int i =0; i < no_of_clients; i++){
        client_id[i] = client_connect(socket_id);
    }
    //int client_id = client_connect(socket_id);
    get_name();
	server_logic();
    close(socket_id);
    //fclose(ptr);
    return 0;    
}