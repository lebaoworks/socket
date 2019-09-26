#ifndef __sock_comm__

#define __sock_comm__
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#define PACKET_SIZE 4096

int send_signal(int sock)
{
    send(sock,&"a",1,0);
}
int recv_signal(int sock)
{
    char a[1];
    recv(sock,a,1,0);
}

//<sock id>
//<pointer to data to send>
//<len to send>
void send_data(int sock, char* data, int len)
{
    //Send data size
    printf("Send 4 bytes: 0x%08x\n",len);
    send(sock,&len,4,0);	
    recv_signal(sock);		                                    //Wait until delivered
    
    //Send data
    printf("Send %d byte(s)... ",len);
    send(sock,data,len,0);
    recv_signal(sock);		                                    //Wait until delivered
    printf("Done.");
}

//<sock id>
//<pointer to block 4 bytes receive data size
//return pointer to received data
char* recv_data(int sock, int* data_size)
{
    //Receive data size
    recv(sock,data_size,4,0);					                
    send_signal(sock);						                    //Delivered
    printf("Receive 4 bytes: 0x%08x\n",*data_size);
    
    //Receive data
    printf("Receive %d byte(s)... ",*data_size);
    char* data = calloc(*data_size,1);
    for (int received = 0; received < *data_size;)		        //repeat fill PACKET_SIZE bytes to data until data is filled up
        received += recv(sock,data+received,PACKET_SIZE,0);
    send_signal(sock);						                    //Delivered
    printf("Done.\n");
    
    return data;
}

#endif
