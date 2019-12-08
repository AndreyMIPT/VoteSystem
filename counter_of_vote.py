#!/usr/bin/env python3

import json
import sys

SIZE_OF_BLOCK = 8 + 1024 + 65 * 2
DATA_STRIDE = 8
DATA_SIZE = 1024

podschet={}

with open(sys.argv[1], "rb") as f:
    byte = f.read(SIZE_OF_BLOCK)
    end = byte[DATA_STRIDE:].find(b'\x00')
    data = json.dumps(byte[DATA_STRIDE:end]
    print(data)
    podschet[data['message']] += 1
print(podschet)
