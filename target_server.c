#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h>

#include "socket.h"
#include <ctype.h> //toupper

void client_handle(char* argv[])
{
    (*(int*)argv[1])++;
    printf("--- Current connection: %d\n",*(int*)argv[1]);
    int sock = *argv[0];

    //communicate
    int size = 0;
    char* data;
    int loop = 1;
    while (loop)
    {
        //receive
        data = recv_data(sock,&size);
        printf("Message: %s\n",data);
        //send
        send_data(sock,argv[2],strlen(argv[2]));

        //if (strncmp("EXIT",data,4))
            loop = 0;
        free(data);
    }

    //decrease current handles
    printf("--- Close connection.\n");
    close(sock);
    (*(int*)argv[1])--;
}

//<arguments count>
//<sys call>
//<client count>
//<server port>
//<message to send>
int main(int argc, char const *argv[])
{
    //Check arguments
    if (argc<4)
        return 1;
    int client_count = atoi(argv[1]);
    int port = atoi(argv[2]);
    
    //Create socket
    int sock = 0; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("Create sock fail.");
        return -1; 
    }

    //Bind our server
    struct sockaddr_in serv_addr; 
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
    
    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Bind fail.");
        return 1; 
    }

    //Listen
    if (listen(sock,client_count)<0)
    {
        printf("Listen fail.");
        return 1;
    }


    printf("Waiting for connections...");
    
    int current_handles = 0;
    int sock_client = 0;
    char* args[3] = {&sock_client,&current_handles,argv[3]}; //sockid,current_handles,message

    pthread_t thread;
    socklen_t addr_size;
    struct sockaddr_in addr;
    
    while (1)
    {
        //if all connections are used up 
        if (current_handles >= client_count)
            continue;
        //if accept success
        if ((sock_client = accept(sock,(struct sockaddr *)&addr, &addr_size))<0)
        {
            printf("Accept fail.\n");
            continue;
        }
        printf("Receive connect from %s.\n",inet_ntoa(addr.sin_addr));

        //if create thread success
        if (pthread_create(&thread,NULL,client_handle,args)!=0)
        {
            printf("Create thread fail.");
            close(sock_client);
            continue;
        }
    }
    //close connection
    close(sock);

    return 0; 
}
