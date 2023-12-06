#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define AES_KEY_SIZE 256
#define AES_BLOCK_SIZE 16

int encryptData(const unsigned char *plaintext, size_t plaintext_len, const unsigned char *key, const unsigned char *iv, unsigned char **ciphertext, size_t *ciphertext_len);
int decryptData(const unsigned char *ciphertext, size_t ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char **plaintext, size_t *plaintext_len);

