#include "blockhain.h"
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

BlockChain::BlockChain()
{
	_filename = "BlockChain" + std::to_string(getpid()) + ".txt";

}

vector<data_to_send> BlockChain::RecordToDisk()
{
	return this->_blockChain;
}

void BlockChain::AddBlock( Block& block) {

	// If the last block was null, it is the first in the chain.
	// Therefore add null to the last block's reference
	struct data_to_send temp = block.generate_data();
	this->_blockChain.push_back(temp);
	SaveBlockToFile( temp);
}

const string BlockChain::GetLastBlockHash()
{
	if(_blockChain.empty())
		return "";
	cout << sizeof(_blockChain.back()._hash) << endl;
	return string(_blockChain.back()._hash, sizeof(_blockChain.back()._hash));
}

void BlockChain::SaveBlockToFile(struct data_to_send data)
{
	/*std::ofstream out;          // поток для записи
	out.open( _filename,std::ofstream::app | std::ofstream::binary);             // окрываем файл для записи
	if (out.is_open())
	{
		out << data._nonce << data._timestamp << data._data << data._hash << data._prevHash; 
	    out.close();
	}*/
	std::ofstream out(_filename, ofstream::app | ios::binary);            //Ставим режим "бинарный файл"
        out.write((char*)&data, sizeof(data));
        out.close();
}
