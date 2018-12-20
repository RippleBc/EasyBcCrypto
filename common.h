#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <gmp.h>

#define NAME_LEN 218
#define FILE_NAME_LEN 218

#define AES_GROUP_SIZE 16
#define AES_SUB_GROUP_SIZE 4

#define AES_ROUND_NUM 12

#define DES_GROUPING_SIZE 8
#define DES_SUB_GROUPING_SIZE (DES_GROUPING_SIZE / 2)

#define DES_ROUND_NUM 12

#define MAX_STR_SIZE (1024 * 8)

#define BYTE_SIZE 8

#define SLASH_N_SIZE 1

#define MILLER_RABIN_TEST_TIMES 40

#define AES_DEBUG (0)
#define RANDOM_DEBUG (1)
#define PRIME_DEBUG (1)
#define DO_POW_MOD_DEBUG (0)
#define RSA_DEBUG (1)
#define GENE_D_DEBUG (0)

#define ECC_DEBUG (0)
#define ECC_GENERATE_KEY_DEBUG (0)
#define ECC_SIGN_DEBUG (0)
#define ECC_VERIFY_SIGN_DEBUG (0)

#define OPT_DEBUG (0)

extern void aes_encrypt(unsigned char *);
extern void aes_decrypt(unsigned char *);

extern void des_encrypt(unsigned char *);
extern void des_decrypt(unsigned char *);

const extern unsigned char init_vector[];

extern void aes_cbc_encrypt(unsigned char *, const int);
extern void aes_cbc_decrypt(unsigned char *, const int);

extern void aes_cfb_encrypt(unsigned char *, const int);
extern void aes_cfb_decrypt(unsigned char *, const int);

extern void aes_ofb_encrypt(unsigned char *, const int);
extern void aes_ofb_decrypt(unsigned char *, const int);

extern mpz_t G_BIG_INT_ZERO;
extern mpz_t G_BIG_INT_ONE;
extern mpz_t G_BIG_INT_TWO;
extern mpz_t G_BIG_INT_THREE;

#endif