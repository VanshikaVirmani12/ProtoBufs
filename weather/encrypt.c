#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "encrypt.h"

int encryptData(const unsigned char *plaintext, size_t plaintext_len, const unsigned char *key, const unsigned char *iv, unsigned char **ciphertext, size_t *ciphertext_len) {
    EVP_CIPHER_CTX *ctx;
    int len; 
    int ciphertext_len_int;

    // Create and initialise the context
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        fprintf(stderr, "EVP_CIPHER_CTX_new() failed!\n");
        return -1;
    }
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    // Allocate buffer for ciphertext
    *ciphertext = (unsigned char *)malloc(plaintext_len + AES_BLOCK_SIZE);
    if (*ciphertext == NULL) {
        fprintf(stderr, "malloc() failed!\n");
        return -1;
    }

    // Perform encryption
    EVP_EncryptUpdate(ctx, *ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len_int = len;

    // Finalise the encryption
    EVP_EncryptFinal_ex(ctx, *ciphertext + len, &len);
    ciphertext_len_int += len;

    *ciphertext_len = (size_t)ciphertext_len_int;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}

// Function for decyprting data
int decryptData(const unsigned char *ciphertext, size_t ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char **plaintext, size_t *plaintext_len) {

    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len_int;

    // Create and initialise the context
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        fprintf(stderr, "EVP_CIPHER_CTX_new() failed!\n");
        return -1;
    }
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    // Allocate buffer for plaintext
    *plaintext = (unsigned char *)malloc(ciphertext_len + AES_BLOCK_SIZE);
    if (*plaintext == NULL) {
        fprintf(stderr, "malloc() failed!\n");
        return -1;
    }

    // Perform decryption
    EVP_DecryptUpdate(ctx, *plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len_int = len;

    // Finalise the decryption
    EVP_DecryptFinal_ex(ctx, *plaintext + len, &len);
    plaintext_len_int += len;

    *plaintext_len = (size_t)plaintext_len_int;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}

int main() {
    // Example key and initialization vector (IV)
    const unsigned char key[] = "0123456789abcdef0123456789abcdef";
    const unsigned char iv[] = "0123456789abcdef";

    // Example data to be encrypted
    const unsigned char plaintext[] = "Hello, World!";
    size_t plaintext_len = strlen((const char *)plaintext);

    // Encrypt the data
    unsigned char *ciphertext;
    size_t ciphertext_len;
    if (encryptData(plaintext, plaintext_len, key, iv, &ciphertext, &ciphertext_len) != 0) {
        fprintf(stderr, "Encryption failed\n");
        return 1;
    }

    // Print the encrypted data
    printf("Encrypted Data: ");
    for (size_t i = 0; i < ciphertext_len; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // Decrypt the data
    unsigned char *decrypted_data;
    size_t decrypted_data_len;
    if (decryptData(ciphertext, ciphertext_len, key, iv, &decrypted_data, &decrypted_data_len) != 0) {
        fprintf(stderr, "Decryption failed\n");
        free(ciphertext);
        return 1;
    }

    // Print the decrypted data
    printf("Decrypted Data: %s\n", decrypted_data);

    // Cleanup
    free(ciphertext);
    free(decrypted_data);

    return 0;
}