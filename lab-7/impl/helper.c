#include "header.h"

void printPacket(struct packet *p){  //always follow this format for printing the packet
    printf("{\n");
    printf("    Version: %d\n", p->version);
    printf("    Header Length: %d\n", p->headerLength);
    printf("    Total Length: %d\n", p->totalLength);
    printf("    Source Department: %d\n", p->srcDept);
    printf("    Destination Department: %d\n", p->destDept);
    printf("    CheckSum: %d\n", p->checkSum);
    printf("    Hops: %d\n", p->hops);
    printf("    Type: %d\n", p->type);
    printf("    ACK: %d\n", p->ACK);
    if(p->headerLength == 6){
        printf("    Source Campus: %d\n", p->srcCampus);
        printf("    Destination Campus: %d\n", p->destCampus);
    }
    printf("    Data: %s\n", p->data);
    printf("}\n");
}



unsigned char* serialize( struct packet* p){
    printf("ttt3 %d\n",sizeof(p));
    unsigned char *buffer = (unsigned char *)malloc(sizeof(p));
    unsigned long long temp = 0;
    int len = 5;
    printf("ttt\n");
    fflush(stdout);
    temp = p->version^temp;
    temp = temp<<4;
    temp = p->headerLength^temp;
    temp = temp<<8;
    temp = p->totalLength^temp;
    temp = temp<<3;
    temp = p->srcDept^temp;
    temp = temp<<3;
    temp = p->destDept^temp;
    temp = temp<<10;
    temp = p->checkSum^temp;
    temp = temp<<3;
    temp = p->hops^temp;
    temp = temp<<3;
    temp = p->type^temp;
    temp = temp<<2;
    temp = p->ACK^temp;
    
    if(p->srcCampus != NULL){
        temp = temp<<4;
        temp = p->srcCampus^temp;
        temp = temp<<4;
        temp = p->destCampus^temp;
        len++;
    }
    
    unsigned char *temprev = (unsigned char *)&temp;
    //printf("temprev = %d\n", temprev[0]);
    //unsigned char *charPointer = (unsigned char *)&temp;
    //strcpy((char *)buffer, p->data);
    
    
    for(int i = 0;i <=len ;i++){
        buffer[i] = temprev[len-i-1];
    }
    for(int i = len ; i < sizeof(p->data)+len;i++){
        buffer[i] = p->data[i-len];
        //printf("- %c \n", p->data[i-len]);
    }

    
    
    for(int i =0; i <8;i++){
        //printf("%d\n", buffer[i]);
    }
    
    
    
    //code for serialization
    return buffer;
}

struct packet *deserialize(unsigned char* buffer){
    // struct packet *p;
    // code for deserialization

    // return p;
}

struct packet *generatePacket(int version, int headerLength, int totalLength, 
                              int srcDept, int destDept, int checkSum, int hops, 
                              int type, int ACK, int srcCampus, int destCampus, char* data) {
    //feel free to write your own function with more customisations. This is a very basic func 
    struct packet *p;
    p = (struct packet *)malloc(sizeof(struct packet));
    p->version = version;
    p->headerLength = headerLength;
    p->totalLength = totalLength;
    p->srcDept = srcDept;
    p->destDept = destDept;
    p->checkSum = checkSum;
    p->hops = hops;
    p->type = type;
    p->ACK = ACK;
    p->srcCampus = srcCampus;
    p->destCampus = destCampus;
    strcpy(p->data, data);
    return p;
}
