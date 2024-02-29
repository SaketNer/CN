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
#include <pthread.h>
#include <stdbool.h>

// File Pointer declared 
//FILE* ptr; 




#define max_clients 1024
#define name_size 1024
#define BUFFER_SIZE 1024
//int no_of_clients = 3;

//delete these 3 lines
int client_id[max_clients];
char name[max_clients][1024];
int clientno = max_clients;


pthread_t thread_id[1024]; 

int client_count = 0;
struct clientEntry {
    int client_fd ;
    char name[name_size];
};

int grp_count = 0;
struct GroupEntry {
    char grp_name[name_size] ;
    int user_ids[BUFFER_SIZE];
    int grp_size;
};

struct clientEntry clients[max_clients];
struct GroupEntry grps[max_clients];

pthread_mutex_t lock_clientEntry_list; 
pthread_mutex_t lock_GrpEntry_list; 

//helper functions starts

void stripNewline(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
}

// Function to split a string into words using ':' as delimiter
char **splitString(const char *input, int *num_words, char delimiter) {
    char *words = (char *)malloc(1024 * sizeof(char *));
    if (words == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    char *token;
    char *copy = strdup(input); // Duplicate input string to avoid modification

    token = strtok(copy, &delimiter);
    *num_words = 0;
    while (token != NULL) {
        words[*num_words] = strdup(token); // Duplicate each token
        (*num_words)++;
        if (*num_words >= 1024) {
            printf("Maximum number of words reached\n");
            break;
        }
        token = strtok(NULL, &delimiter);
    }

    free(copy); // Free the duplicated string
    return words;
}

void send_data(char* msg, int client_fd){
    int send_count;
	if((send_count = send(client_fd, msg, strlen(msg), 0)) == -1){
		perror("send");
		close(client_fd);
		//close(new_server_sockfd);
        exit(1);
	}
    return;
}

int find_client_by_name(char* name){
    for (int i = 0; i < max_clients; i++)
    {
        if(strcmp(clients[i].name, name)==0){
            return i;
        }
    }
    return -1;
}

bool valid_client_by_name(int pos){
    if(clients[pos].client_fd==-1){
        return false;
    }
    return true;
}

//helper functions end


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
	if(listen(server_sockfd, 1024) == -1){ // 1024 indicates the number of pending connections
        perror("listen");
        close(server_sockfd);
        exit(1);
    }
	return server_sockfd;

}

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


int get_name(int client_fd){
    char msg[BUFFER_SIZE];
    char reply[BUFFER_SIZE];
    memset(msg, 0, BUFFER_SIZE);
    memset(reply, 0, BUFFER_SIZE);
    int recv_count;
    strcpy(msg,"NAME\n");

        // 6. SEND
	int send_count;
	if((send_count = send(client_fd, msg, strlen(msg), 0)) == -1){
		perror("send");
		close(client_fd);
		//close(new_server_sockfd);
        exit(1);
	}

    //RECEIVE
    if((recv_count = recv(client_fd, reply, 1024, 0)) == -1){
        perror("recv");
        close(client_fd);
        //close(new_server_sockfd);
        exit(1);
    }
    stripNewline(reply);
    int pos = 0;
    pthread_mutex_lock(&lock_clientEntry_list); 
    clients[client_count].client_fd = client_fd;
    strcpy(clients[client_count].name, reply);
    pos = client_count;
    client_count++;
    pthread_mutex_unlock(&lock_clientEntry_list);
    printf("Client %s connected\n", reply);
    return pos;
	
}

void * server_logic(void *i){
    int client_fd = (int) i;
    int clientEntry_pos = get_name(client_fd);
    char server_reply[BUFFER_SIZE];
    char client_msg[BUFFER_SIZE];
    while(1){
        memset(server_reply, 0, BUFFER_SIZE);
        memset(client_msg, 0, BUFFER_SIZE);

        //RECEIVE
        int recv_count;
        if((recv_count = recv(client_fd, client_msg, 1024, 0)) == -1){
            perror("recv");
            close(client_fd);
            //close(new_server_sockfd);
            exit(1);
        }
        
        printf("Client %s: %s", clients[clientEntry_pos].name, client_msg);
        if(strcmp(client_msg,"LIST\n")==0||strcmp(client_msg,"LIST")==0){
            for (int i = 0; i < max_clients; ++i)
            {
                if (clients[i].client_fd == -1) continue;
                strcat(server_reply, clients[i].name);
                strcat(server_reply, ":");
            }
            server_reply[strlen(server_reply) - 1] = '\n';
            send_data(server_reply, client_fd);
            printf("SENDING: Client %s: %s\n", clients[clientEntry_pos].name, server_reply);
            fflush(stdout);
        }

        else if(strncmp(client_msg,"MSGC",4)==0){
            
            char msgc[BUFFER_SIZE], receiver_name[BUFFER_SIZE], actual_message[BUFFER_SIZE];
            sscanf(client_msg, "%[^:]:%[^:]:%[^\n]", msgc, receiver_name, actual_message);
            int pos = find_client_by_name(receiver_name);
            if(pos ==-1){
                printf("USER NOT FOUND\n");
                fflush(stdout);
                continue;
            }
            strcat(server_reply, clients[clientEntry_pos].name);
            strcat(server_reply, ":");
            strcat(server_reply, actual_message);
            server_reply[strlen(server_reply) ] = '\n';
            send_data(server_reply, clients[pos].client_fd);
            printf("SENDING from %s to %s: %s\n", clients[clientEntry_pos].name,clients[pos].name, server_reply);
            fflush(stdout);
        }

        else if(strncmp(client_msg, "GRPS", 4) == 0){
            char command_name[BUFFER_SIZE], group_members[BUFFER_SIZE], group_name[BUFFER_SIZE];
            sscanf(client_msg, "%[^:]:%[^:]:%[^\n]", command_name, group_members, group_name);
            struct GroupEntry temp_grp;
            strcpy(temp_grp.grp_name,group_name);
            temp_grp.grp_size= 0;
            bool valid_group = true;
            char *members = strtok(group_members, ",");

            while(members!=NULL && valid_group){
                int pos = find_client_by_name(members);
                bool valid_client = valid_client_by_name(pos);
                if(!valid_client) {
                    valid_group = false;
                    printf("INVALID USERS LIST\n");
                    fflush(stdout);
                    continue;
                }
                temp_grp.user_ids[temp_grp.grp_size] = clients[pos].client_fd;
                temp_grp.grp_size++;
            }

            pthread_mutex_lock(&lock_GrpEntry_list);
            strcpy(grps[grp_count].grp_name, temp_grp.grp_name);




        }
        
    }
}

int main(int argc, char *argv[])
{   
    if (argc != 3)
	{
		printf("Use 2 cli arguments\n");
		return -1;
	}
    //ptr = fopen("./Hello.txt", "w"); 
    
    for(int i =0 ; i<max_clients; i++){
        clients[i].client_fd = -1;
    }
	
	// extract the address and port from the command line arguments
	// extract the address and port from the command line arguments
	char addr[INET6_ADDRSTRLEN];
	strcpy(addr, argv[1]);
	int port = atoi(argv[2]);

	/*int socket_id = create_connection(addr, port);
    for(int i =0; i < no_of_clients; i++){
        client_id[i] = client_connect(socket_id);
    }*/
    int socket_id = create_connection(addr, port);
    while(1){
        if(client_count<=max_clients){
            int client_fd =  client_connect(socket_id);
            pthread_t thread_id; 
            pthread_create(&thread_id, NULL, server_logic, (void *) client_fd); 
        }
    }


    close(socket_id);
    return 0;    
}