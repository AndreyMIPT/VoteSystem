#ifndef KEYS_H_INCLUDED
#define KEYS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

int create_keys(EVP_PKEY *private_key, EVP_PKEY *public_key);

#endif
