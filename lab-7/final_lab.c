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

int checkSumCalc(unsigned char* buffer){
    int sum = 0;
    for(int i = 0;i<sizeof(buffer);i++){
        
    }
}

unsigned char* serialize( struct packet* p){
    //printf("ttt3 %d\n",sizeof(p));
    unsigned char *buffer = (unsigned char *)malloc(sizeof(p));
    unsigned long long temp = 0;
    int len = 5;
    //printf("ttt\n");
    //fflush(stdout);
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
    
    if(p->headerLength == 6){
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
    
    //for(int i =0; i <8;i++){
        //printf("%d\n", buffer[i]);
   // }
    
    //code for serialization
    return buffer;
}

struct packet *deserialize(unsigned char* buffer){
    struct packet *p = (struct packet *)malloc(sizeof(struct packet));
    // code for deserialization

    p->version = buffer[0]>>4;
    //printf("version = %d\n", p->version);
    p->headerLength = buffer[0]&0b00001111;
    //printf("headerLength = %d\n", p->headerLength);
    p->totalLength = buffer[1]&0b11111111;
    //printf("totalLength = %d\n", p->totalLength);
    p->srcDept = buffer[2]>>5;
    //printf("srcDept = %d\n", p->srcDept);
    p->destDept = (buffer[2]&0b00011100)>>2;
    //printf("destDept = %d\n", p->destDept);
    long long temp = (((buffer[2]&00000011)<<8)&0b1100000000|buffer[3]);
    p->checkSum = temp ;
    //printf("checkSum = %d\n", p->checkSum);
    p->hops = buffer[4]>>5;
    //printf("hops = %d\n", p->hops);
    p->type = (buffer[4]&0b00011100)>>2;
    //printf("type = %d\n", p->type);
    p->ACK = buffer[4]&0b00000011;
    if(p->headerLength == 6){
        p->srcCampus = buffer[5]>>4;
        //printf("srcCampus = %d\n", p->srcCampus);
        p->destCampus = buffer[5]&0b00001111;
        //printf("destCampus = %d\n", p->destCampus);
    }
    
    for(int i = 0; i < 1024; i++){
        p->data[i] = buffer[i+p->headerLength];
    }
    //p->data[6]='\n';
    //printf("data = %s\n", p->data);
    return p;
}