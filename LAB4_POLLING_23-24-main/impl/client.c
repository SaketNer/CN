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
char name[1024];

int create_connection(char* addr, int port) {
    int client_sockfd = socket(AF_INET , SOCK_STREAM, 0);
    if(client_sockfd==-1){
		printf("socket creation failed\n");
		return -1;
	}
    struct sockaddr_in server_addrinfo;
	server_addrinfo.sin_family = AF_INET;
    server_addrinfo.sin_port = htons(port);

    //converts the character string src into a network address structure 
    //in the af address family, then copies the network address structure to dst
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

void send_data(int socket_id) {
	memset(msg, 0, 1024);
	//fgets(msg, 1024, stdin);
    //fflush(stdin);
	
    if(strcmp(reply, "NAME\n") == 0 ||strcmp(reply, "NAME") == 0){
        printf("INITIALIZING......\n");
        strcpy(msg, name);
    }
    if(strcmp(reply, "POLL\n") == 0 || strcmp(reply, "POLL") == 0){
        printf("ENTER CMD: ");
        fgets(msg, 1024, stdin);
        fflush(stdin);
        if(strcmp(msg,"EXIT\n")==0||strcmp(msg,"EXIT")==0){
            printf("CLIENT TERMINATED: EXITING......\n");
        }else if(strcmp(msg, "NOOP") == 0||strcmp(msg, "NOOP\n") == 0){

        }
        else if(strncmp(msg, "MESG",4) == 0 ||strncmp(msg, "MESG",4) == 0 ){
            /*recv_data(socket_id);
            char temp[1024];
            memset(temp, 0, 1024);
            int currep = 0;
            int curtemp = 0;
            int cnt = 1;
            while(reply[currep] != '\n'){
                if(reply[currep] == ':'){
                    printf("%d. %s",cnt,temp);
                    memset(temp, 0, 1024);
                    curtemp = 0;
                    cnt++;
                }else{
                    temp[curtemp]=reply[currep];
                }
                currep++;
                curtemp++;
                
            }*/
        }
        else if(strcmp(msg, "LIST\n") == 0||strcmp(msg, "LIST") == 0){

        }
        else{
            memset(msg, 0, 1024);
            strcpy(msg, "INVALID CMD\n");
        }
        //strcpy(msg, name);
    }
	int send_count = send(socket_id, msg, strlen(msg), 0);
	if(send_count==-1){
		printf("Message not sent\n");
		close(socket_id);
		exit(1);
	}
    if(strcmp(msg,"EXIT\n")==0||strcmp(msg,"EXIT")==0){
        close(socket_id);
        exit(1);
    }
    if(strcmp(msg, "LIST\n") == 0||strcmp(msg, "LIST") == 0){
        recv_data(socket_id);
        char temp[1024];
        memset(temp, 0, 1024);
        int currep = 0;
        int curtemp = 0;
        int cnt = 1;
        //printf("%s",reply);
        while(reply[currep] != '\n'){
            if(reply[currep] == ':' || reply[currep] == '\n'){
                printf("%d. %s\n",cnt,temp);
                fflush(stdout);
                memset(temp, 0, 1024);
                curtemp = -1;
                cnt++;
            }else{
                temp[curtemp]=reply[currep];
            }
        currep++;
        curtemp++;
                
        }
        printf("%d. %s\n",cnt,temp);
        fflush(stdout);
        memset(temp, 0, 1024);
    }
}




int main(int argc, char *argv[])
{
    if (argc != 4)
	{
		printf("Use 3 cli arguments\n");
		return -1;
	}
	
    
	// extract the address and port from the command line arguments
	char addr[INET6_ADDRSTRLEN];
	strcpy(addr, argv[1]);
	unsigned int port = atoi(argv[2]);
    strcpy(name, argv[3]);
    char end = '\n';
    strncat(name, &end, 1);
	int socket_id = create_connection(addr, port);

	while (1)
    {
        recv_data(socket_id);
        send_data(socket_id);
    }
}