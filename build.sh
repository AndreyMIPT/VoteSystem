#!/bin/bash

g++ server.cpp block.cpp keys.cpp base64.cpp -lssl -lcrypto -lpthread -std=c++11
