#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP
#include "block.h"
#include <vector>
#include <string>
#include <fstream>

class BlockChain
{
public:
	BlockChain();
	void AddBlock( Block& block);
	vector<data_to_send> RecordToDisk();
	const std::string GetLastBlockHash();
	void SaveBlockToFile(struct data_to_send data);
private:
	std::vector<data_to_send> _blockChain;
	std::string _filename;
};
#endif