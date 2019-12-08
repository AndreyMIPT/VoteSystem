#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <memory>
#include "block.h"
#include "blockhain.h"
#include <thread>
#include <poll.h>
#include <fstream>
#define PORT 8080

using namespace std;

/* Описание функции First_Thread. 
   
   Функция ничего не возвращает.
   
   В качестве аргумента передаем блок (block), данные от сервера (data),
   дескриптор серверного сокета (listen_socket)
*/

void First_Thread(struct data_to_send* data, volatile bool& flag_prish, int socket)
{
	int res = recv(socket, data, sizeof(struct data_to_send), 0);
	if( res > 0)
	{
	    std::cout << "HashToServer " << data->_hash << std::endl;
	    flag_prish = 1;
	}else
	exit(0);
}

void record_thread(BlockChain bc, int socket)
{
	int ret = 0;
	struct pollfd fd;
	fd.fd = socket;
    fd.events = POLLHUP;
	while( ret == 0)
	{	
		// ждём до 10 секунд
    	ret = poll( &fd, 1, 10000 );
    	// Проверяем успешность вызова
    	if ( ret == 0  ) {continue;}
    	else if ( ret == -1 ) {return;}
    	// таймаут, событий не произошло
    	else
    	{
    		std::ofstream myout;          // поток для записи
    		myout.open("BlockChain.txt");             // окрываем файл для записи
    		if (myout.is_open())
    		{
    			vector<data_to_send> v = bc.RecordToDisk();
    			for (auto t : v)
    			{
    				myout << t._nonce << t._data << t._hash << t._prevHash; 
    			}
    		    myout.close();
    		}

    	}
    }
}

int main(int argc, char* argv[])
{
	int sock = 0, valread; 
    struct sockaddr_in serv_addr;
    data_to_send data_1;
    char hello[] = "Hello from client"; 
    char buffer[1024] = {0}; 
	volatile bool flag_prish = 0;    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        cout << "\n Socket creation error \n"; 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        cout << "\nInvalid address/ Address not supported \n"; 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout << "\nConnection Failed \n"; 
        return -1; 
    } 

	
	/* Create the blockchain */

	BlockChain bc;
	thread thread_0(record_thread, bc, sock);
	while (1)
	{
		Block block;
        
		char buffer[1024] = {0}; 
		recv( sock, buffer, 1024 , 0); 

		flag_prish = 0;
		
		data_1 = block.generate_data();
		thread thread_1(First_Thread, &data_1, ref(flag_prish), sock);
        
        Block self_block(buffer);
        cout << bc.GetLastBlockHash() << endl;
        self_block.Mine(flag_prish, bc.GetLastBlockHash());

		if (flag_prish == 0)
		{
			auto tmp = self_block.generate_data();
			send(sock, &tmp, sizeof(tmp), 0);
		}
		
		while (flag_prish == 0)
		{

		}
        //cout << "HashBefore = " << data_1._hash << std::endl;

        block.From_Data_To_Block(data_1);

        //cout << "HashAfter = " << block.GetHash() << std::endl;
        
		bc.AddBlock(block);
		thread_1.join();
	}
	
	// Show combined output
	//cout << bc->GetLastBlock()->GetHash() << endl;
	//cout << bc->GetLastBlock()->GetData() << endl;

	return 0;
}