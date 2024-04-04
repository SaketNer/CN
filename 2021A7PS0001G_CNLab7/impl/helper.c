#include "header.h"

void printPacket(struct packet *p){  //always follow this format for printing the packet
    //printf("{\n");
    //printf("    Version: %d\n", p->version);
    //printf("    Header Length: %d\n", p->headerLength);
    //printf("    Total Length: %d\n", p->totalLength);
    //printf("    Source Department: %d\n", p->srcDept);
    //printf("    Destination Department: %d\n", p->destDept);
    //printf("    CheckSum: %d\n", p->checkSum);
    //printf("    Hops: %d\n", p->hops);
    //printf("    Type: %d\n", p->type);
    //printf("    ACK: %d\n", p->ACK);
    if(p->headerLength == 6){
        //printf("    Source Campus: %d\n", p->srcCampus);
        //printf("    Destination Campus: %d\n", p->destCampus);
    }
    //printf("    Data: %s\n", p->data);
    //printf("}\n");
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


int checkSumCalc(unsigned char* buffer){
    int sum = 0;
    int len = buffer[1];

    //printf("MYFUN LEN = %d\n", len);
    for(int i = 0;i<len;i++){
        
        if(i == 2){
            sum+=(buffer[2]>>2);
        }
        else if(i==3){
            continue;
        }
        else{
            sum+=buffer[i];
        }
        //printf("sum = %d\n", sum);
        if(((sum&0b1111110000000000)>>10)>0){
            sum = (sum&0b0000001111111111)+1;
        }
        if((sum&0b0000001111111111) == 0b0000001111111111||(sum&0b0000011111111111) == 0b0000011111111111){
            //printf("MYFUN Checksum = %d\n", sum);
            break;
        }
        
    }
    sum = 0b0000001111111111 - sum+1;
    //printf("MYFUN sum = 0x%02x\n", sum);
    ////printf("MYFUN Checksum = %d\n", sum);
    return sum;
}

unsigned char* serialize( struct packet* p){
    //#//printf("ttt3 %d\n",sizeof(p));
    unsigned char *buffer = (unsigned char *)malloc(sizeof(p));
    unsigned long long temp = 0;
    int len = 5;
    //#//printf("ttt\n");
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
    //temp = p->checkSum^temp;
    temp = 0^temp;
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
    //#//printf("temprev = %d\n", temprev[0]);
    //unsigned char *charPointer = (unsigned char *)&temp;
    //strcpy((char *)buffer, p->data);
    
    
    for(int i = 0;i <=len ;i++){
        buffer[i] = temprev[len-i-1];
        
    }
    
    
    for(int i = len ; i < sizeof(p->data)+len;i++){
        buffer[i] = p->data[i-len];
        ////printf("- %c \n", p->data[i-len]);
    }
    
    //for(int i =0; i <8;i++){
        ////printf("%d\n", buffer[i]);
   // }
    
    //code for serialization
    unsigned int checksumval = checkSumCalc(buffer);
    buffer[2]=buffer[2]|(checksumval>>8);
    buffer[3]=checksumval&0b11111111;   
    return buffer;
}

struct packet *deserialize(unsigned char* buffer){
    struct packet *p = (struct packet *)malloc(sizeof(struct packet));
    // code for deserialization

    p->version = buffer[0]>>4;
    ////printf("version = %d\n", p->version);
    p->headerLength = buffer[0]&0b00001111;
    ////printf("headerLength = %d\n", p->headerLength);
    p->totalLength = buffer[1]&0b11111111;
    ////printf("totalLength = %d\n", p->totalLength);
    p->srcDept = buffer[2]>>5;
    ////printf("srcDept = %d\n", p->srcDept);
    p->destDept = (buffer[2]&0b00011100)>>2;
    ////printf("destDept = %d\n", p->destDept);
    long long temp = (((buffer[2]&00000011)<<8)&0b1100000000|buffer[3]);
    p->checkSum = temp ;
    ////printf("checkSum = %d\n", p->checkSum);
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

/*int checksumCalc(unsigned char* buffer){
    int checksum = 0;
    int ptr = 0;
 
    // version and header length
    checksum += buffer[ptr];
    ptr++;
 
    // total length
    checksum += buffer[ptr];
    ptr++;
 
    //source and dest dept, except first 2 bits of checksum
    checksum += (buffer[ptr] >> 2);
    ptr++;
 
    // ignore the last 8 bits of checksum
    ptr++;
 
    // hops, type and ack
    checksum += buffer[ptr];
    ptr++;
 
    //src and dest campus
    if (buffer[1] == 6) {
        checksum += buffer[ptr];
        ptr++;
    }
 
    //data
    for (int i = 0; i < BUFFER_SIZE; i++){
        checksum += buffer[ptr];
        ptr++;
    }
 
    // 2's complement
    checksum = ~checksum + 1;
 
    return checksum;
}*/

/*struct packet *generatePacket(int version, int headerLength, int totalLength, 
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
}*/
unsigned char* getPacket(char* input, int srcCampus, int srcDept, int* validDept[3], int numOfValidDept[3]){
    int number = input[0] - '0';
    struct packet* p;
 
    if (number == 1 || number == 2){
        p = generateUnicastPacket(input, srcCampus, srcDept, validDept, numOfValidDept);
    }
    else if (number == 3 || number == 4){
        p = generateBroadcastPacket(input, srcCampus, srcDept, validDept, numOfValidDept);
    }
    else if (number == 5){
        p = generateControlPacket(input, srcCampus, srcDept, validDept, numOfValidDept);
    }
    else{
        return NULL;
    }
 
    unsigned char* buffer = (unsigned char*)malloc(sizeof(struct packet));
    buffer = serialize(p);
 
    return buffer;
}
 
unsigned char* generateUnicastPacket(char* input, int srcCampus, int srcDept, int* validDept[3], int numOfValidDept[3]){
    int number = input[0] - '0';
    
    int destCampus = -1, destDept = -1;
    char payload[BUFFER_SIZE];
    
    if (number == 1){
        sscanf(input, "%*[^.].%*[^:]:%*[^:]:%*[^\n]", NULL, &destCampus, &destDept, payload);
    }
    else if (number == 2) {
        sscanf(input, "%*[^.].%*[^:]:%*[^\n]", NULL, &destDept, payload);
    }
 
    // check if srcDept is valid for srcCampus
    bool found = false;
    for (int i = 0; i < numOfValidDept[srcCampus]; i++){
        if (validDept[srcCampus][i] == srcDept){
            found = true;
            break;
        }
    }
 
    if (!found){
        return NULL;
    }
 
    // check if destDept is valid for destCampus
    // if destCampus is -1, then check if destDept valid for srcCampus
    found = false;
    if (destCampus != -1){
        for (int i = 0; i < numOfValidDept[destCampus]; i++){
            if (validDept[destCampus][i] == destDept){
                found = true;
                break;
            }
        }
    }
    else{
        for (int i = 0; i < numOfValidDept[srcCampus]; i++){
            if (validDept[srcCampus][i] == destDept){
                found = true;
                break;
            }
        }
    }
 
    if (!found){
        return NULL;
    }
 
    // set the packet
    struct packet* p = (struct packet*)malloc(sizeof(struct packet));
 
    // version
    p->version = 2;
 
    // header length
    if (destCampus == -1){
        //intra-campus
        p->headerLength = 5;
    }
    else{
        //inter-campus
        p->headerLength = 6;
    }
 
    // src and dest dept
    p->srcDept = srcDept;
    p->destDept = destDept;
 
    // src and dest campus
    p->srcCampus = srcCampus;
    if (p->headerLength == 5){
        //intra-campus
        p->destCampus = 0;
    }
    else{
        //inter-campus
        p->destCampus = destCampus;
    }
 
    // dont cares - hops, ack and type
    p->hops = 0;
    p->type = 0;
    p->ACK = 0;
    
    // data
    strcpy(p->data, payload);
 
    // total length
    p->totalLength = p->headerLength + strlen(p->data);
 
    // checksum
    // p->checkSum = checksumCalc(serialize(p)) & ((1 << 10) - 1);
 
    return serialize(p);
}
 
unsigned char* generateBroadcastPacket(char* input, int srcCampus, int srcDept, int* validDept[3], int numOfValidDept[3]){
    int number = input[0] - '0';
    
    char payload[BUFFER_SIZE];
    sscanf(input, "%*[^.].%*[^\n]", NULL, payload);
 
    // check if srcDept is valid for srcCampus
    bool found = false;
    for (int i = 0; i < numOfValidDept[srcCampus]; i++){
        if (validDept[srcCampus][i] == srcDept){
            found = true;
            break;
        }
    }
    if (!found){
        return NULL;
    }
 
    struct packet* p = (struct packet*)malloc(sizeof(struct packet));
    p->version = 2;
    
    if (number == 3){
        p->headerLength = 5;
    }
    else {
        p->headerLength = 6;
    }
    
    // DOUBT - HOW TO KNOW WHICH SIDE IS SERVER - SERVER SIDE DEPT IS 0
    p->srcDept = srcDept;
    p->destDept = 0;
 
    p->srcCampus = srcCampus;
    if (number == 3){
        p->destCampus = 0b111;
    }
    else if (number == 4){
        p->destCampus = 0b1111;
    }
 
    // dont cares - hops, ack and type
    p->hops = 0;
    p->type = 0;
    p->ACK = 0;
    
    // data
    strcpy(p->data, payload);
 
    // total length
    p->totalLength = p->headerLength + strlen(p->data);
 
    // checksum
    // p->checkSum = checksumCalc(serialize(p)) & ((1 << 10) - 1);
 
    return serialize(p);
}
 
unsigned char* generateControlPacket(char* input, int srcCampus, int srcDept, int* validDept[3], int numOfValidDept[3]){
    // check if srcDept is valid for srcCampus
    bool found = false;
    for (int i = 0; i < numOfValidDept[srcCampus]; i++){
        if (validDept[srcCampus][i] == srcDept){
            found = true;
            break;
        }
    }
    if (!found){
        return NULL;
    }
 
    struct packet* p = (struct packet*)malloc(sizeof(struct packet));
    // set the packet
 
    // version
    p->version = 2;
    
    // header length
    p->headerLength = 5;
 
    // src and dest dept - DOUBT
    p->srcDept = srcDept;
    p->destDept = 0;
 
    // src and dest campus - dont exist for intra-campus packets
    p->srcCampus = 0;
    p->destCampus = 0;
 
    // dont cares - hops, ack
    p->hops = 0;
    p->ACK = 0;
 
    // type
    p->type = 1;
 
    // total length
    p->totalLength = p->headerLength;
    
    // checksum
    // p->checkSum = checksumCalc(serialize(p)) & ((1 << 10) - 1);
 
    return serialize(p);
}