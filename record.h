#include <poll.h>
#include "block.h"
#include "blockhain.h"
#define file "BlockChain.out"

void record_thread(Blockchain bc, int socket)
{
	int ret = 0;
	struct pollfd fd;
	fd.fd = socket;
    fd.events = POLLHUP;
	while( ret == 0)
	{	
		// ждём до 10 секунд
    	ret = poll( fd, 1, 10000 );
    	// Проверяем успешность вызова
    	if ( ret == 0  ) {continue;}
    	else if ( ret == -1 ) {return;}
    	// таймаут, событий не произошло
    	else
    	{
    		std::ofstream out;          // поток для записи
    		out.open(file);             // окрываем файл для записи
    		if (out.is_open())
    		{
    		    out << bc->RecordToDisk();
    		}
    	}
    }
}