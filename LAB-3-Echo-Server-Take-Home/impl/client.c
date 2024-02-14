#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

char msg[1024];
char reply[1024];

// Create a connection to the server
int create_connection(char* addr, int port) {
	int client_sockfd = socket(AF_INET , SOCK_STREAM, 0);
	if(client_sockfd==-1){
		//printf("socket creation failed\n");
		return -1;
	}
	struct sockaddr_in server_addrinfo;
	server_addrinfo.sin_family = AF_INET;
    server_addrinfo.sin_port = htons(port);

	if (inet_pton(AF_INET, addr, &server_addrinfo.sin_addr) <= 0) {
       //printf("\nInvalid address/ Address not supported \n");
        close(client_sockfd);
        exit(1);
    }

	if(connect(client_sockfd, (struct sockaddr*)&server_addrinfo, sizeof(server_addrinfo)) == -1){ // client connects if server port has started listen()ing and queue is non-full; however server connects to client only when it accept()s
        printf("Could not find server");
        close(client_sockfd);
        exit(1);
    }
	return client_sockfd;
}

void send_data(int socket_id) {
	memset(msg, 0, 1024);
	fgets(msg, 1024, stdin);
	if(strcmp(msg,"EXIT\n")==0 || strcmp(msg, "EXIT") == 0){
		printf("Client exited successfully");
		close(socket_id);
		exit(1);
	}
	int send_count = send(socket_id, msg, strlen(msg), 0);
	if(send_count==-1){
		//printf("Message not sent\n");
		close(socket_id);
		exit(1);
	}
}

// Receive input from the server
void recv_data(int socket_id) {
	int recv_count;
	memset(reply, 0, 1024);
	if((recv_count = recv(socket_id, reply, 1024, 0)) == -1){
            //perror("recv");
			close(socket_id);
            exit(1);
    }
	//printf("Server: %s\n", reply);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
	{
		printf("Use 2 cli arguments\n");
		return -1;
	}
	
    
	// extract the address and port from the command line arguments
	char addr[INET6_ADDRSTRLEN];
	strcpy(addr, argv[1]);
	unsigned int port = atoi(argv[2]);

	int socket_id = create_connection(addr, port);

	while (1)
    {
        send_data(socket_id);
        recv_data(socket_id);
    }
}