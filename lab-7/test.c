#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct packet {
    int version;
    int headerLength;
    int totalLength;
    int srcDept;
    int destDept;
    int checkSum;
    int hops;
    int type;
    int ACK;
    int srcCampus;
    int destCampus;
    char data[1024];
};


unsigned char* serialize( struct packet* p){
    unsigned char *buffer = (unsigned char *)malloc(sizeof(p));
    long long temp = 0;
    temp = p->destCampus^temp;
    temp = temp<<4;
    temp = p->srcCampus^temp;
    temp = temp<<2;
    temp = p->ACK^temp;
    temp = temp<<3;
    temp = p->type^temp;
    temp = temp<<3;
    temp = p->hops^temp;
    temp = temp<<10;
    temp = p->checkSum^temp;
    temp = temp<<3;
    temp = p->destDept^temp;
    temp = temp<<3;
    temp = p->srcDept^temp;
    temp = temp<<1;
    temp = p->totalLength^temp;
    temp = temp<<4;
    temp = p->headerLength^temp;
    temp = temp<<4;
    temp = p->version^temp;
    
    /*temp = temp^p->version;
    temp = temp^ (p->headerLength<<4);
    temp = temp^ (p->headerLength<<4);
    temp = temp^ (p->totalLength<<8);
    temp = temp^ (p->srcDept<<16);
    temp = temp^ (p->destDept<<19);
    temp = temp^ (p->checkSum<<22);
    temp = temp^ (p->hops<<32);
    temp = temp^ (p->type<<35);
    temp = temp^ (p->ACK<<38);
    temp = temp^ (p->srcCampus<<40);
    temp = temp^ (p->destCampus<<44);*/
    char *charPointer = (char *)&temp;
    //printf("%c\n", charPointer[2]);
    for(int i =0; i <6;i++){
        printf("%c\n", charPointer[i]);
    }
    
    
    
    //code for serialization
    return charPointer;
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


int main() {
    printf("Hello\n");
    /*char *buffer = (char *)malloc(10);
    long long test = 0;
    test = test^0x323130;
    test = test<<16;
    char *charPointer = (char *)&test;
    printf("%c\n", charPointer[2]);*/
    printf("TEST 1:\n");
    struct packet *p;
    p = (struct packet *)malloc(sizeof(struct packet));
    p = generatePacket( 1, 6, 11, 2, 3, 300, 1, 2, 3, 4, 2, "Hello");
    unsigned char ans[11] = {22, 11, 77, 44, 43, 66, 72, 101, 108, 108, 111};
    unsigned char *buffer = serialize(p);
    return 0;
}