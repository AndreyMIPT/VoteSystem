#include "block.h"
#include <iostream>
#include <fstream>

using namespace std;
int main(int argc, char const *argv[])
{
	cout << sizeof(struct data_to_send) << endl;
 	ifstream file; // создаем объект класса ifstream
    file.open(argv[1],std::ifstream::binary);
    struct data_to_send data;
    file.read((char*)&data,sizeof(data));
    cout << "hash= " << data._hash << "data=" << data._data << "nonce=" << data._nonce << "_prevHash" << data._prevHash;
    cout << endl;
    file.read((char*)&data,sizeof(data));
    cout << "hash= " << data._hash << "data=" << data._data << "nonce=" << data._nonce << "_prevHash" << data._prevHash;
    cout << endl;
	return 0;
}