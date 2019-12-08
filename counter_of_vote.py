#!/home/andrey/anaconda3/bin/python3

import json
import sys

SIZE_OF_BLOCK = 8 + 2048 + 65 * 2
DATA_STRIDE = 8
DATA_SIZE = 2048

podschet={}

with open(sys.argv[1], "rb") as f:
    while True:
        byte = f.read(SIZE_OF_BLOCK)
        if not byte:
            break
        end = byte[DATA_STRIDE + 1:].find(b'\x00')
        SIZE = end + DATA_STRIDE + 1
        struct = byte[DATA_STRIDE:SIZE].decode('utf8') 
        data = json.loads(struct)
        try:
            vote = podschet[data['Message']]
        except KeyError:
            vote = 0
        podschet[data['Message']] = vote + 1
print(podschet)
