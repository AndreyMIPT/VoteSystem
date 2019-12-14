CC=g++
CFLAGS=-c -std=c++11

build: blockchain

blockchain: blockchain.o block.o keys.o Kripta.o
	mkdir -p bin
	$(CC) blockchain.o block.o keys.o Kripta.o -o bin/blockchain -lssl -lcrypto -lpthread
	rm *.o

blockchain.o: blockchain.cpp
	$(CC) $(CFLAGS) blockchain.cpp

block.o: block.cpp
	$(CC) $(CFLAGS) block.cpp

keys.o: keys.cpp
	$(CC) $(CFLAGS) keys.cpp

Kripta.o: Kripta.cpp
	$(CC) $(CFLAGS) Kripta.cpp

run: bin/blockchain
	./bin/blockchain

clean:
	rm *.o
	rm -Rdf bin
