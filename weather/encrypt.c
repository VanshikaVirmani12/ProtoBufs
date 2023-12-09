#include <stdio.h>
#include <string.h>

#include "encrypt.h"

#include <openssl/err.h>

int generate_rsa_key(EVP_PKEY **pkey, int key_size) {
  EVP_PKEY_CTX *pctx = NULL; 
  
  pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

  if (!pctx) {
    fprintf(stderr, "Error creating EVP_PKEY_CTX.\n");
    return 1;
  }

  if (EVP_PKEY_keygen_init(pctx) <= 0) {
    fprintf(stderr, "Error initializing EVP_PKEY key generation context.\n");
    EVP_PKEY_CTX_free(pctx);
    return 1;
  }

      // Set RSA key size
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(pctx, key_size) <= 0) {
        fprintf(stderr, "Error setting RSA key size.\n");
        EVP_PKEY_CTX_free(pctx);
        return 1;
    }


    // Generate the RSA key pair
    if (EVP_PKEY_keygen(pctx, pkey) <= 0) {
        fprintf(stderr, "Error generating RSA key pair.\n");
        EVP_PKEY_CTX_free(pctx);
        return 1;
    }

    EVP_PKEY_CTX_free(pctx);

    return 0;

} 

void generate_keys(EVP_PKEY *pkey) {
  EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_from_name(NULL, "RSA", NULL);

  EVP_PKEY_keygen_init(pctx);
  EVP_PKEY_generate(pctx, &pkey);
  EVP_PKEY_CTX_free(pctx);
  // EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
  //
  // if (EVP_PKEY_keygen_init(pctx) <= 0 ||
  //     EVP_PKEY_CTX_set_rsa_keygen_bits(pctx, 2048) <= 0 ||
  //     EVP_PKEY_generate(pctx, &pkey) <= 0) {
  //   fprintf(stderr, "Error generating RSA key pair\n");
  //   EVP_PKEY_CTX_free(pctx);
  //   exit(1);
  // }
  //
  // EVP_PKEY_CTX_free(pctx);
}

long get_private_key(EVP_PKEY *pkey, char **private_key_copy) {
  BIO *bio = BIO_new(BIO_s_mem());
  if (!PEM_write_bio_PrivateKey(bio, pkey, NULL, NULL, 0, NULL, NULL)) {
    printf("Error writing private key data in memory BIO\n");
    return 0;
  }
  char *private_key;
  long private_key_length = BIO_get_mem_data(bio, &private_key);
  *private_key_copy = malloc(private_key_length + 1);
  memcpy(*private_key_copy, private_key, private_key_length);
  (*private_key_copy)[private_key_length] = '\0';
  BIO_free_all(bio);
  return private_key_length;
}

long get_public_key(EVP_PKEY *pkey, char **public_key_copy) {
  BIO *bio = BIO_new(BIO_s_mem());
  if (!PEM_write_bio_PUBKEY(bio, pkey)) {
    printf("Error writing public key data in memory BIO\n");
    return 0;
  }
  char *public_key;
  long public_key_length = BIO_get_mem_data(bio, &public_key);
  *public_key_copy = malloc(public_key_length + 1);
  memcpy(*public_key_copy, public_key, public_key_length);
  (*public_key_copy)[public_key_length] = '\0';
  BIO_free_all(bio);
  return public_key_length;
}

EVP_PKEY *get_public_key_from_string(char *public_key_string) {
  BIO *bio = BIO_new(BIO_s_mem());
  BIO_puts(bio, public_key_string);
  EVP_PKEY *pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
  BIO_free_all(bio);
  return pkey;
}

EVP_PKEY *get_private_key_from_string(char *private_key_string) {
  BIO *bio = BIO_new(BIO_s_mem());
  BIO_puts(bio, private_key_string);
  EVP_PKEY *pkey = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
  BIO_free_all(bio);
  return pkey;
}

void encrypt_message(EVP_PKEY *pkey, char *message, char **encrypted_message,
                     size_t *encrypted_message_length) {
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
  if (!ctx) {
    printf("Error creating context\n");
    return;
  }
  if (EVP_PKEY_encrypt_init(ctx) <= 0) {
    printf("Error initializing encryption\n");
    return;
  }
  if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
    printf("Error setting padding\n");
    return;
  }
  if (EVP_PKEY_encrypt(ctx, NULL, encrypted_message_length,
                       (unsigned char *)message, strlen(message)) <= 0) {
    printf("Error getting encrypted message length\n");
    return;
  }
  unsigned char *ciphertext = OPENSSL_malloc(*encrypted_message_length);
  if (!ciphertext) {
    printf("Error allocating memory for encrypted message\n");
    return;
  }
  int retval;
  if ((retval = EVP_PKEY_encrypt(ctx, ciphertext, encrypted_message_length,
                       (unsigned char *)message, strlen(message))) <= 0) {
    ERR_print_errors_fp(stderr);
    printf("%d\n", retval);
    printf("Error encrypting message\n");
    return;
  }
  *encrypted_message = malloc(*encrypted_message_length);
  memcpy(*encrypted_message, ciphertext, *encrypted_message_length);
  OPENSSL_free(ciphertext);
  EVP_PKEY_CTX_free(ctx);
}

void decrypt_message(EVP_PKEY *pkey, char *encrypted_message,
                     char **decrypted_message, size_t *decrypted_message_length,
                     size_t encrypted_message_length) {
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
  if (!ctx) {
    printf("Error creating context\n");
    return;
  }
  if (EVP_PKEY_decrypt_init(ctx) <= 0) {
    printf("Error initializing decryption\n");
    return;
  }
  if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
    printf("Error setting padding\n");
    return;
  }
  if (EVP_PKEY_decrypt(ctx, NULL, decrypted_message_length,
                       (unsigned char *)encrypted_message,
                       encrypted_message_length) <= 0) {
    printf("Error getting decrypted message length\n");
    return;
  }
  unsigned char *plaintext = OPENSSL_malloc(*decrypted_message_length);
  if (!plaintext) {
    printf("Error allocating memory for decrypted message\n");
    return;
  }
  if (EVP_PKEY_decrypt(ctx, plaintext, decrypted_message_length,
                       (unsigned char *)encrypted_message,
                       encrypted_message_length) <= 0) {
    printf("Error decrypting message\n");
    return;
  }
  *decrypted_message = malloc(*decrypted_message_length + 1);
  memcpy(*decrypted_message, plaintext, *decrypted_message_length);
  (*decrypted_message)[*decrypted_message_length] = '\0';
  OPENSSL_free(plaintext);
  EVP_PKEY_CTX_free(ctx);
}

// int main() {
//   EVP_PKEY *pkey = EVP_PKEY_new();
//   generate_keys(pkey);
//   char *priv_k = NULL;
//   char *pub_k = NULL;
//   long priv_k_length, pub_k_length;
//   priv_k_length = get_private_key(pkey, &priv_k);
//   pub_k_length = get_public_key(pkey, &pub_k);
//   // printf("Private key:\n%s\nLength: %lu\n", priv_k, priv_k_length);
//   // printf("Public key:\n%s\nLength: %lu\n", pub_k, pub_k_length);
//
//   EVP_PKEY *pkey_priv = get_private_key_from_string(pub_k);
//   EVP_PKEY *pkey_pub = get_public_key_from_string(pub_k);
//
//   // EVP_PKEY *pkey3 = EVP_PKEY_new();
//   // generate_keys(pkey3);
//   //
//   // printf("%d\n", EVP_PKEY_eq(pkey, pkey2));
//   // printf("%d\n", EVP_PKEY_eq(pkey, pkey3));
//
//   char *encrypted_message = NULL;
//   size_t encrypted_message_length;
//   encrypt_message(pkey_pub, "Hello, world!", &encrypted_message,
//                   &encrypted_message_length);
//   printf("Encrypted message length: %zu\n", encrypted_message_length);
//   // printf("Encrypted message: %s\n", encrypted_message);
//
//   char *decrypted_message = NULL;
//   size_t decrypted_message_length;
//   decrypt_message(pkey, encrypted_message, &decrypted_message,
//                   &decrypted_message_length, encrypted_message_length);
//   printf("Decrypted message length: %zu\n", decrypted_message_length);
//   printf("Decrypted message: %s\n", decrypted_message);
//
//   free(priv_k);
//   free(pub_k);
//   EVP_PKEY_free(pkey);
// }
