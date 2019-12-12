#include "keys.h"

int create_keys(char *private_key, char *public_key)
{
	RSA *rsa = RSA_generate_key(2048, 3, 0, 0);
	BIO *bio = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL);

	int keylen = BIO_pending(bio);
	BIO_read(bio, private_key, keylen);

	PEM_write_bio_RSAPublicKey(bio, rsa);
	keylen = BIO_pending(bio);
	BIO_read(bio, public_key, keylen);
	
	BIO_free_all(bio);
	RSA_free(rsa);

	return 0;
}
