#!/usr/bin/env python3
import socket
import time

TCP_IP = '127.0.0.1'
TCP_PORT = 8888
BUFFER_SIZE = 512
HASH_SIZE = 64

lst = [1, 0] * (BUFFER_SIZE)
msg_arr = bytearray(lst)

#msg_arr = "0000aabbccddee";
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
while True:
    s.send(msg_arr)
    data = s.recv(HASH_SIZE)
    time.sleep(10.0)
s.close()
