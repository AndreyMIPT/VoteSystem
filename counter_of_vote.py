#!/usr/bin/env python3

import json
import sys

SIZE_OF_BLOCK = 1610
DATA_STRIDE = 8+64
DATA_SIZE = 1024

podschet={}

with open(sys.argv[1], "rb") as f:
	byte = f.read(SIZE_OF_BLOCK)
        for i in range(DATA_STRIDE, DATA_SIZE):
        
