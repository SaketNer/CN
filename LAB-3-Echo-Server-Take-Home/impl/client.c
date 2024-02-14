#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Create a connection to the server
int create_connection(char* addr, int port) {

}

void send_data(int socket_id) {

}

// Receive input from the server
void recv_data(int socket_id) {

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

	while (1)
    {
        send_data(socket_id);
        recv_data(socket_id);
    }
}