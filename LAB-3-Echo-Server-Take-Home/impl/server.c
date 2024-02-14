#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


// create connction
int create_connection(char* addr, int port) {

}

// Accept incoming connections
int client_connect(int socket_id) {

}

// Echo input from client
void echo_input(int socket_id) {

}

int main(int argc, char *argv[])
{
    if (argc != 3)
	{
		printf("Use 2 cli arguments\n");
		return -1;
	}
	
	// extract the address and port from the command line arguments

	int socket_id = create_connection(addr, port);
    int client_id = client_connect(socket_id);
	echo_input(client_id);
    close(socket_id);
    return 0;    
}
