#ifndef BLOCK_HPP
#define BLOCK_HPP
#include <string>
#include <string.h>
#include <memory>
#include <functional>


using namespace std;

struct data_to_send
{
	int64_t _nonce; /// The counter for mining
	//char _timestamp[64]; /// The timestamp of creation
	char _data[2048*8]; /// The smart contract
	char _hash[65]; /// The current hash
	char _prevHash[65];
} __attribute((packed));

class Block
{
public:


	Block();
	Block(const char* data);
	/*!
	 * \brief Returns this block's hash
	 * \return SHA-256 hash
	 */
	string GetHash() const;

	/*!
	 * \brief Sets the previous block
	 *  @param block The block to be added
	 */
	void SetPrevBlock(Block* block);

	/*!
	 * \brief Get previous block
	 */
	 
	 Block* GetPrevBlock(Block* block);

	/*!
	 * \brief Tells if the current hash is correct according to format
	 *        0000abcderfgt
	 * \return
	 */
	bool IsValid(uint16_t difficulty);

	/*!
	 * \brief Mines the block
	 */
	void Mine(volatile bool& flag_prish, const string& last);

	string GetData();

	struct data_to_send generate_data() const;

	void From_Data_To_Block( struct data_to_send);

private:
	int64_t _nonce; /// The counter for mining
	//string _timestamp; /// The timestamp of creation
	char _data[2048*8]; /// The smart contract
	string _hash; /// The current hash
	string _prevHash;

	/*!
	 * \brief CreateHash
	 * \return The sha-256 hash as a string
	 */
	string CreateHash() const;
    
};

#endif // BLOCK_HPP