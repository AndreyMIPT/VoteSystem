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
#include "keys.h"
#include "base64.h"

#define PORT 8888
#define NUM_OF_BLOCKCH 3
#define BLOCKCH_PORT 8080
#define SIZE_OF_DATA 2048
#define SIZE_OF_CRYPT_DATA SIZE_OF_DATA * 8//2^3*2048
#define PART_DATA 128// it has to be power of 2
#define CRYPT_DATA 256
#define SIZE_OF_OK sizeof(bool)
#define SIZE_OF_HASH 64
#define SIZE_OF_BLOCK sizeof(struct data_to_send)

int cmp( const void* a, const void* b)
{
    key_send_t* m =(key_send_t*)a;
    key_send_t* n =(key_send_t*)b;
    return n->pid - m->pid;
}

void cryptDATA(char* input, char* output, key_send_t* key)
{
    char output0[SIZE_OF_DATA*2];
    for (int i = 0; i < SIZE_OF_DATA/PART_DATA; ++i) {
        encrypt_msg(key[0].public_key, input + PART_DATA*i, output0 + CRYPT_DATA*i, PART_DATA);
    }
    char output1[SIZE_OF_DATA*4];
    for (int i = 0; i < 2*SIZE_OF_DATA/PART_DATA; ++i) {
        encrypt_msg(key[1].public_key, output0 + PART_DATA*i, output1 + CRYPT_DATA*i, PART_DATA);
    }
    for (int i = 0; i < 4*SIZE_OF_DATA/PART_DATA; ++i) {
        encrypt_msg(key[2].public_key, output1 + PART_DATA*i, output + CRYPT_DATA*i, PART_DATA);
    }
}

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket;
    int blockch_fd, blockch_socket[NUM_OF_BLOCKCH];
    struct sockaddr_in address, blockch_addr; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char data[SIZE_OF_DATA] = {};
    char cryptData[SIZE_OF_CRYPT_DATA] = {};
    std::string hello = "Hello from server";
    int ret;
    std::string hash;
    struct data_to_send block;
    key_send_t key[NUM_OF_BLOCKCH];
    
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
    for (int j = 0; j < NUM_OF_BLOCKCH ; ++j) {
        recv( blockch_socket[j] , &key[j], sizeof(key[j]), 0);
        cout << key[j].pid << key[j].public_key << endl;
     };
    std::qsort(key, NUM_OF_BLOCKCH, sizeof(key_send_t), cmp);
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

        cryptDATA(data,cryptData,key);

        auto base64_crypt_data_string = base64_encode((unsigned char *)cryptData, strlen(cryptData));
        char base64_crypt_data[SIZE_OF_CRYPT_DATA] = {};
        strcpy(base64_crypt_data, base64_crypt_data_string.c_str());
        std::cout << base64_crypt_data << std::endl;

        for (int i = 0; i < NUM_OF_BLOCKCH; ++i)
        {
            send(blockch_socket[i] , cryptData, SIZE_OF_CRYPT_DATA , 0);
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

