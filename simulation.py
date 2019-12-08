#!/usr/bin/env python3
import socket
import time
import json

TCP_IP = '127.0.0.1'
TCP_PORT = 8888
BUFFER_SIZE = 1024
HASH_SIZE = 64

lst = [1, 0] * (BUFFER_SIZE)
msg_arr = bytearray(lst)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
while True:
    s.send(msg_arr)
    data = s.recv(HASH_SIZE)
    time.sleep(5.0)
s.close()
