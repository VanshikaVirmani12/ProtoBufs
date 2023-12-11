#ifndef VV_PROTOBUF_ENC_H
#define VV_PROTOBUF_ENC_H

#include <openssl/evp.h>
#include <openssl/pem.h>

int encrypt_protobuf(char *plaintext, size_t plaintext_len, char *key, char *iv,
                     char **ciphertext, size_t *ciphertext_len);
int decrypt_protobuf(char *ciphertext, size_t ciphertext_len, char *key,
                     char *iv, char **plaintext, size_t *plaintext_len);

#endif
