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
#include "keys.h"
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
    cout << "Anrdey" << res << " data" << data->_data[9] << endl;
	if( res > 0)
	{
	    std::cout << "HashToServer " << data->_hash << std::endl;
	    flag_prish = 1;
	}else
	exit(0);
}

int main(int argc, char* argv[])
{
	int sock = 0, valread; 
    struct sockaddr_in serv_addr;
    data_to_send data_1;
    char hello[] = "Hello from client";
    char buffer[2048] = {0};
	volatile bool flag_prish = 0;

	char private_key[MAX_KEY_SIZE] = {};
	char public_key[MAX_KEY_SIZE] = {};

	create_keys(private_key, public_key);
	key_send_t key;
	key.pid = getpid();
	strcpy(key.public_key, public_key);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "\n Socket creation error \n";
        return -1; 
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "192.168.0.165", &serv_addr.sin_addr)<=0)
    {
        cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "\nConnection Failed \n";
        return -1;
    }

	printf("Sending...\n");
	printf("%s\n", key.public_key);
	printf("pid = %d\n", key.pid);

	send(sock, &key, sizeof(key), 0);

	printf("Sent successful!\n");
	
	/* Create the blockchain */

	BlockChain bc;
	while (1)
	{
		Block block;
        
		char buffer[2048] = {0};
		recv( sock, buffer, 2048 , 0);

		flag_prish = 0;
		
		data_1 = block.generate_data();
		thread thread_1(First_Thread, &data_1, ref(flag_prish), sock);
        
        Block self_block(buffer);
        cout << bc.GetLastBlockHash() << endl;
        self_block.Mine(flag_prish, bc.GetLastBlockHash());

		if (flag_prish == 0)
		{
			auto tmp = self_block.generate_data();
			cout << tmp._data << endl;
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
