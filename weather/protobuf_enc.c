#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include "protobuf_enc.h"

int encrypt_protobuf(char *plaintext, size_t plaintext_len, char *key, char *iv, char **ciphertext, size_t *ciphertext_len) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len_int;

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        fprintf(stderr, "Error creating context.\n");
        return 1;
    }

    // Initialize the encryption operation with AES-256-CBC and the provided key and IV
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        fprintf(stderr, "Error initializing encryption.\n");
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    *ciphertext = malloc(plaintext_len + EVP_CIPHER_CTX_block_size(ctx));
    if (!*ciphertext) {
        perror("malloc");
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    // Perform the encryption
    if (1 != EVP_EncryptUpdate(ctx, *ciphertext, &len, plaintext, plaintext_len)) {
        fprintf(stderr, "Error getting encrypted message.\n");
        free(*ciphertext);
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    ciphertext_len_int = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, *ciphertext + len, &len)) {
        fprintf(stderr, "Error finalising Encryption.\n");
        free(*ciphertext);
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    ciphertext_len_int += len;
    *ciphertext_len = (size_t)ciphertext_len_int;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}

int decrypt_protobuf(char *ciphertext, size_t ciphertext_len, char *key, char *iv, char **plaintext, size_t *plaintext_len) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len_int;

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        printf("Error creating context\n");
        return 1;
    }

    // Initialize the decryption operation with AES-256-CBC and the provided key and IV
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        printf("Error initializing decryption\n");
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    *plaintext = malloc(ciphertext_len);
    if (!*plaintext) {
        perror("malloc");
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    // Perform the decryption
    if (1 != EVP_DecryptUpdate(ctx, *plaintext, &len, ciphertext, ciphertext_len)) {
        printf("Error decrypting message\n");
        free(*plaintext);
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    plaintext_len_int = len;

    // Finalize the decryption
    if (1 != EVP_DecryptFinal_ex(ctx, *plaintext + len, &len)) {
        printf("Error finalising decryption length\n");
        free(*plaintext);
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }

    plaintext_len_int += len;
    *plaintext_len = (size_t)plaintext_len_int;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}