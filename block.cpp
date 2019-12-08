#define _CRT_SECURE_NO_WARNINGS

#include "block.h"
#include <chrono>
#include <iostream>
#include "picosha2.h"
#include <vector>

/// Creates a hash from this and the previous block
string Block::CreateHash() const {

//	string accumulated_string = to_string(_nonce) + string(_prevHash, 64) + string(_data, 1024);
	// Create a non-const char*
//	char* cstr = new char[accumulated_string.length() + 1];
//	cstr[accumulated_string.length()] = '\0';
//	strcpy(cstr, accumulated_string.c_str());
//	cout << _prevHash << endl;
	auto tmp = generate_data();
	string res;
	picosha2::hash256_hex_string((unsigned char*)&tmp, (unsigned char*)(&tmp+1), res);
	return res;
	//return SHA256(cstr);
}

/// Constructor
Block::Block():
	_data{}
{
	//this->_timestamp = to_string(std::chrono::system_clock::now().time_since_epoch().count());
//	CreateHash();
}

Block::Block(const char* data)
{
	memcpy(_data, data, sizeof(_data));
}

/// Returns this block's hash
string Block::GetHash() const {
	return _hash;
}

/// Sets the previous block
//void Block::SetPrevBlock(Block* block) {
//	_prevBlock = block;
//}

///Gets the previous block
Block* Block::GetPrevBlock(Block* block){
	return block->GetPrevBlock( block);
}

/// Checks if the block is valid
bool Block::IsValid(uint16_t difficulty) {

	string hashed = GetHash();

	if (hashed.length() == 0)
		return false;

	vector<char> require;
	for (int i = 0; i < difficulty; ++i) {
		require.push_back('0');
	}
	require.push_back('\0');
	string require_str (require.begin(), require.end());

	// Check if the first few digits are all zeros
	if (hashed.substr(0, difficulty) == require_str.substr(0, difficulty)) {
		return true;
	}
	else 
		return false;
}

void Block::Mine(volatile bool& flag_prish, const string& last) {

	_prevHash = last;
	_nonce = 0;
	_hash = CreateHash();
	while (!(IsValid(4) || flag_prish)) {
		_nonce++;
		_hash = CreateHash();
	}
	//std::cout << "hash_2 = " << _hash << std::endl;
}

string Block::GetData() {
	return _data;
}

void Block::From_Data_To_Block( struct data_to_send d)
{
    memcpy(_data, d._data, sizeof(d._data));
	this->_hash = d._hash;
	this->_nonce = d._nonce;
	this->_prevHash = d._prevHash;
	//this->_timestamp = d._timestamp;
}

struct data_to_send Block::generate_data() const
{
    data_to_send t;
    t._nonce = _nonce;
    //strcpy(t._timestamp, this->_timestamp.c_str());
    memcpy(t._data, this->_data, sizeof(t._data));
    strcpy(t._hash, this->_hash.c_str());
    strcpy(t._prevHash, this->_prevHash.c_str());
    return t;
}

