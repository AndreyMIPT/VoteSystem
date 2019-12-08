#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <fcntl.h>
#include <poll.h>
#include <memory>
#include <chrono>
#include "block.h" 

#define PORT 8888 
#define NUM_OF_BLOCKCH 3
#define BLOCKCH_PORT 8080
#define SIZE_OF_DATA 2048
#define SIZE_OF_OK sizeof(bool)
#define SIZE_OF_HASH 64
#define SIZE_OF_BLOCK sizeof(struct data_to_send)

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket;
    int blockch_fd, blockch_socket[NUM_OF_BLOCKCH];
    struct sockaddr_in address, blockch_addr; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char data[SIZE_OF_DATA] = {}; 
    std::string hello = "Hello from server";
    int ret;
    std::string hash;
    struct data_to_send block;
    
    struct pollfd fds[NUM_OF_BLOCKCH];
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    for (int i = 0; i < NUM_OF_BLOCKCH; ++i)
    {
        if ((blockch_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        { 
            perror("socket failed"); 
            exit(EXIT_FAILURE); 
        }  
    }
       

    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
    if (setsockopt(blockch_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    blockch_addr.sin_family = AF_INET; 
    blockch_addr.sin_addr.s_addr = INADDR_ANY; 
    blockch_addr.sin_port = htons(BLOCKCH_PORT);

    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    if (bind(blockch_fd, (struct sockaddr *)&blockch_addr,  
                                 sizeof(blockch_addr))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }  
    


    //conect to Ivan
    if (listen(server_fd, 1) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 


if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }

//conect to SASHA
    if (listen(blockch_fd, NUM_OF_BLOCKCH) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 


for (int i = 0; i < NUM_OF_BLOCKCH; ++i)
{
    
    if ((blockch_socket[i] = accept(blockch_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
    // от blockch_socket[i] мы будем ожидать входящих данных
    std::cout << "Good connect" << endl;
    fds[i].fd = blockch_socket[i];
    fds[i].events = POLLIN;
}
//workspace
while(1)
    {
        //ret = 0;

        int res = 0;
        while( res < SIZE_OF_DATA)
        {
            int gotit = recv( new_socket , data + res, SIZE_OF_DATA - res, 0);
            res += gotit;
            if (gotit <= 0)
                return 1;
        }
        std::cout << "recive data=" << data << endl;
        for (int i = 0; i < NUM_OF_BLOCKCH; ++i)
        {
            send(blockch_socket[i] , data , SIZE_OF_DATA , 0);
            printf("data send\n");
        }
        //while( ret == 0)
        //{
            // ждём до 10 секунд
            ret = poll( fds, NUM_OF_BLOCKCH, 100000);
            // Проверяем успешность вызова
            if ( ret == -1 ) {return 5;}
            else if ( ret == 0 ) {continue;}
            // таймаут, событий не произошло
            else
            for (int i = 0; i < NUM_OF_BLOCKCH; ++i)
            {
                if (fds[i].revents & POLLIN)
                {
                    recv( blockch_socket[i] , &block, SIZE_OF_BLOCK, 0);

                    std::cout << "HashInServer" << block._hash << std::endl;
                    break;
                }
            }
        //}
        for (int i = 0; i < NUM_OF_BLOCKCH; ++i)
            {
                std::cout << "HashFromServer" << block._hash << std::endl;
                send( blockch_socket[i] , &block, SIZE_OF_BLOCK, 0);
            }
        hash = block._hash;
        send( new_socket , hash.c_str() , SIZE_OF_HASH,0); 
        printf("hash= %s\n", hash.c_str());
    
        
        for (int i = 0; i < NUM_OF_BLOCKCH; ++i)
        {
            recv( blockch_socket[i] , &block, SIZE_OF_BLOCK, MSG_DONTWAIT);
        }

        
    }
    return 0; 
} 

