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

#define MAX_KEY_SIZE 10000

typedef struct key_send_s
{
	pid_t pid;
	char public_key[MAX_KEY_SIZE];
} key_send_t;

int create_keys(char *private_key, char *public_key);
int encrypt_msg(const char *public_key, const char *msg, char *result);

#endif
