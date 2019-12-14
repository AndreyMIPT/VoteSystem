#!./env/bin/python

import json
import sys 
from Crypto.PublicKey import RSA 
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Util import number
from Crypto import Random

class Counter:
    BLOCK_SIZE = 8 + 2048 * 8 + 65 * 2 
    DATA_STRIDE = 8 
    DATA_SIZE = 2048 * 8 

    def __init__(self):
        self._key_1 = RSA.importKey(open('../../Pr_key9609.pem', 'r').read())
        self._key_2 = RSA.importKey(open('../../Pr_key9608.pem', 'r').read())
        self._key_3 = RSA.importKey(open('../../Pr_key9607.pem', 'r').read())
        self._votes = {}

    def _decrypt_block(self, block):
        data = b''
        for i in range(16):
            decryptor_1 = PKCS1_OAEP.new(self._key_1)
            decryptor_2 = PKCS1_OAEP.new(self._key_2)
            decryptor_3 = PKCS1_OAEP.new(self._key_3)
            offset = 1024 * i
            data_1 = decryptor_3.decrypt(block[offset:offset + 256])
            data_2 = decryptor_3.decrypt(block[offset + 256:offset + 256 * 2])
            data_3 = decryptor_3.decrypt(block[offset + 256 * 2:offset + 256 * 3])
            data_4 = decryptor_3.decrypt(block[offset + 256 * 3:offset + 256 * 4])
            data_2_1 = decryptor_2.decrypt(data_1 + data_2)
            data_2_2 = decryptor_2.decrypt(data_3 + data_4)
            data += decryptor_1.decrypt(data_2_1 + data_2_2)
        return data.decode('utf-8')

    def count(self, filename):
        with open(filename, 'rb') as f:
            encrypted_block = f.read(self.BLOCK_SIZE)
            while encrypted_block:
                block_json = self._decrypt_block(encrypted_block[8:8 + 1024 * 16])
                block = json.loads(block_json[:1042])
                try:
                    vote = self._votes[block['Message']]
                except KeyError:
                    vote = 0
                self._votes[block['Message']] = vote + 1
                encrypted_block = f.read(self.BLOCK_SIZE)

    def get_votes(self):
        return self._votes


counter = Counter()
counter.count(sys.argv[1])
print(counter.get_votes())
