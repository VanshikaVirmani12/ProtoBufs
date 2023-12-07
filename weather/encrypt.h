#ifndef VV_ENCRYPT_H
#define VV_ENCRYPT_H

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <sys/_types/_size_t.h>

void generate_keys(EVP_PKEY *pkey);
long get_private_key(EVP_PKEY *pkey, char **private_key_copy);
long get_public_key(EVP_PKEY *pkey, char **public_key_copy);
EVP_PKEY *get_public_key_from_string(char *public_key_string);
EVP_PKEY *get_private_key_from_string(char *private_key_string);
void encrypt_message(EVP_PKEY *pkey, char *message, char **encrypted_message,
                     size_t *encrypted_message_length);
void decrypt_message(EVP_PKEY *pkey, char *encrypted_message,
                     char **decrypted_message, size_t *decrypted_message_length,
                     size_t encrypted_message_length);

#endif // VV_ENCRYPT_H
