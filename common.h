#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <gmp.h>
#include <stdio.h>
#include <string.h>

#define NAME_LEN 218
#define FILE_NAME_LEN 218
#define MAX_STR_SIZE (1024 * 8)
#define BYTE_SIZE 8
#define SLASH_N_SIZE 1

#define AES_GROUP_SIZE 16
#define AES_SUB_GROUP_SIZE 4
#define AES_ROUND_NUM 12

#define DES_GROUPING_SIZE 8
#define DES_SUB_GROUPING_SIZE (DES_GROUPING_SIZE / 2)
#define DES_ROUND_NUM 12

#define MILLER_RABIN_TEST_TIMES 40

#define TEST_BEGIN (1)

#define DEBUG (0)

#define AES_DEBUG (0)
#define RANDOM_DEBUG (0)
#define PRIME_DEBUG (0)
#define DO_POW_MOD_DEBUG (0)
#define RSA_DEBUG (0)
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

/* random */
extern void InitRandom(int seed_size);
extern void DoGetPositiveRandBigInt(const int if_fix_len, const int byteLen, mpz_t random);
extern void DoGetPositiveOddRandBigInt(const int if_fix_len, const int byteLen, mpz_t random);
extern void DoGetPositiveRand(mpz_t n, mpz_t result);

/* prime */
extern void DoGenRandomPrime(const int if_fix_len, const int byteLen, mpz_t prime);

/* ecc */
extern void GenerateEccKey(const char *const key_pair_file);
extern void EccSign(const unsigned char *const s_source, const char *const key_pair_file, unsigned char *s_r, int *s_r_size, unsigned char *s_s, int *s_s_size);
extern int EccVerifySign(const unsigned char *const s_source, const char *const key_pair_file, const char *const s_r, const int s_r_size, const char *const s_s, const int s_s_size);

/* rsa */
extern void DoGenerateRsaKey(const int byteLen, const char *const key_pair_file);
extern void RsaEncrypt(const unsigned char *const source, const int source_size, unsigned char *dest, int *dest_size, const char *const key_pair_file);
extern void RsaDecrypt(const unsigned char *const source, const int source_size, unsigned char *dest, int *dest_size, const char *const key_pair_file);
extern void RsaSign(const unsigned char *const source, int source_size, unsigned char *dest, int *dest_size, const char *const key_pair_file);
extern int RsaVerifySign(const unsigned char *const source, const int source_size, unsigned char *origin, int origin_size, const char *const key_pair_file);

/*  */
extern void GetBigIntFromFile(const FILE *const p_file, mpz_t big_int);
#endif