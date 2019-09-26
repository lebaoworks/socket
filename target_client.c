#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include "socket.h"

//<arguments count>
//<sys call>
//<server address>
//<server port>
//<message to send>
int main(int argc, char const *argv[]) 
{
    //Check arguments
    if (argc<4)
        return 1;
    char* target_addr = argv[1];
    int port = atoi(argv[2]);
    
    //Connect to server
    int sock = 0; 
    struct sockaddr_in serv_addr; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("Create sock fail.");
        return -1; 
    }
   
    serv_addr.sin_addr.s_addr = inet_addr(target_addr);
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
       
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connect fail.");
        return 1; 
    }

    printf("Connected.\n");

    //communicate
    int size = 0;
    char* data;
    int loop = 1;
    while (loop)
    {
        //send
        send_data(sock,argv[3],strlen(argv[3]));
        //receive
        data = recv_data(sock,&size);
        printf("Message: %s\n",data);
        
        //if (strncmp("EXIT",data,4))
            loop = 0;
        free(data);
    }
    
    int len = PACKET_SIZE;
    
    //close connection
    close(sock);

    return 0; 
}
