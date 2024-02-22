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

// Echo input from client
void echo_input(int socket_id) {
	while(1){
		memset(msg, 0, 1024);
        memset(reply, 0, 1024);
		char temp[1024];
		memset(temp, 0, 1024);
		// 5. RECEIVE
		int recv_count;
		if((recv_count = recv(socket_id, msg, 1024, 0)) == -1){
			perror("recv");
			close(socket_id);
			//close(new_server_sockfd);
			exit(1);
		}	
		
		// 6. SEND
		//int i;
		//for (i = 0; msg[i] != '\0'; ++i);
		//printf("recv_count: %d ,i : %d \n", strlen(msg),i);
		//if(strlen(msg)<5){
		int numBytes = 5;
		int tempBytes = 0;
		if(!(msg[0]=='E' && msg[1]=='C'&& msg[2]=='H'&& msg[3]=='O'&& msg[4]==':')){
			strcpy(reply, "Invalid command: Unable to echo!");
		}
		else{
			strcpy(reply, "OHCE:");
			int i;
			for(i =5 ; i <1023 && msg[i]!='\0';i++){
				if(msg[i]==':'){
					//temp[i-5]='\0';
					break;
				}
				temp[i-5]=msg[i];
			}
			i++;
			for(; i <1023 && msg[i]!='\0';i++){
				if(msg[i]>='0' && msg[i]<='9'){
					tempBytes= tempBytes*10+(msg[i]-'0');
					//printf("checking:%d,val :%d\n", msg[i],tempBytes);
				}
				else if(msg[i]=='-'){
					tempBytes=-1;
					break;
				}
				else{
					break;
				}
				
				
			}
			if(tempBytes!=-1){
				if(tempBytes==0){
					tempBytes=numBytes;
				}
				int j=0;
				int cnt=0;
				int total = tempBytes;
				for(; j<tempBytes;j++){
					if(temp[j]=='\0'){
						break;
					}
					reply[j+5]=temp[j];
					cnt++;
					total--;
					//printf("TOAL : %d\n",total);
				}
				//j++;
				//printf("HERE1 : %d\n",total);
				if(total!=0){
					reply[j+5]=' ';
					j++;
					reply[j+5]='(';
					j++;
					reply[j+5]=cnt+'0';
					j++;
					//char t[20] = [' ','b','y','t','e','s' ,' ','s' ,'e','n','t',')'];
					char t[20] =" bytes sent)";
					for(int r = 0;r<=11;r++){
						reply[j+5+r]=t[r];
					}
					//printf("HERE : %d\n",total);
					reply[j+5+12]='\0';
				}
				else{
					reply[j+7]='\0';
				}
				//printf("TOAL FIANL : %d\n",total);
			}
			else{
				strcpy(reply, "Error: Negative number of bytes");
			}
			

			
		}
		
		
		/*if(strlen(msg)<5){
			strcpy(reply, "Error: Message length must be more than 5 characters");
		}else{
			strcpy(reply, msg);
		}*/
		//printf("sending : %s\n", reply);
		//printf("TEMP : %s ,BYTES :%d\n", temp,tempBytes);

		// 6. SEND
		int send_count;
		if((send_count = send(socket_id, reply, strlen(reply), 0)) == -1){
			perror("send");
			close(socket_id);
			//close(new_server_sockfd);
			exit(1);
		}
		//printf("Client messaged: %s\n", msg);
		//printf("Server echoed: %s\n\n", reply);
	}

}

int main(int argc, char *argv[])
{
    if (argc != 3)
	{
		printf("Use 2 cli arguments\n");
		return -1;
	}
	
	// extract the address and port from the command line arguments
	// extract the address and port from the command line arguments
	char addr[INET6_ADDRSTRLEN];
	strcpy(addr, argv[1]);
	int port = atoi(argv[2]);

	int socket_id = create_connection(addr, port);
    int client_id = client_connect(socket_id);
	echo_input(client_id);
    close(socket_id);
    return 0;    
}