#ifndef _COMMON_H_
#define _COMMON_H_

#define NAME_LEN 218
#define FILE_NAME_LEN 218

#define AES_GROUP_SIZE 16
#define AES_SUB_GROUP_SIZE 4

#define AES_ROUND_NUM 12

#define DES_GROUPING_SIZE 8
#define DES_SUB_GROUPING_SIZE (DES_GROUPING_SIZE / 2)

#define DES_ROUND_NUM 12

#define MAX_STR_SIZE 1024 * 10

#define BYTE_SIZE 8

#define SLASH_N_SIZE 1

#define AES_DEBUG (0)
#define RANDOM_DEBUG (0)
#define MILLER_RABIN_DEBUG (0)
#define DO_POW_MOD_DEBUG (0)
#define RSA_DEBUG (0)
#define GENE_D_DEBUG (0)

#define ECC_DEBUG (1)

#define MILLER_RABIN_TEST_TIMES 5

extern void aes_encrypt(unsigned char *);
extern void aes_decrypt(unsigned char *);

extern void des_encrypt(unsigned char *);
extern void des_decrypt(unsigned char *);

const extern unsigned char init_vector[];

extern void aes_cbc_encrypt(unsigned char *, int);
extern void aes_cbc_decrypt(unsigned char *, int);

extern void aes_cfb_encrypt(unsigned char *, int);
extern void aes_cfb_decrypt(unsigned char *, int);

extern void aes_ofb_encrypt(unsigned char *, int);
extern void aes_ofb_decrypt(unsigned char *, int);

void get_specified_size_random(unsigned char *, int);


#define BIG_INT_BIT_LEN 4096
#define SIGN_BIT BIG_INT_BIT_LEN - 1
#define NUMBER_BIT_LEN BIG_INT_BIT_LEN
#define BUFFER_SIZE BIG_INT_BIT_LEN
#define POSITIVE 0
#define NEGATIVE 1

/* 大整数类型，均用补码表示 */
typedef struct 
{
    unsigned char bit[BIG_INT_BIT_LEN];
} BigInt;

/* Number均用真值表示 */
typedef struct    
{
    unsigned char value[NUMBER_BIT_LEN]; /* 数字数组 */
    int len; /* 数组长度 */
    int sign; /* 符号标记 */
} Number;

extern char* ChangeStringRadix(const char *const, const int, const int, char *);
extern BigInt* CopyBigInt(const BigInt *const, BigInt *);
extern BigInt* ShiftArithmeticLeft(const BigInt *const, const int indent, BigInt *);
extern BigInt* ShiftArithmeticRight(const BigInt *const, const int indent, BigInt *);
extern BigInt* DoAdd(const BigInt *const, const BigInt *const, BigInt *);
extern BigInt* DoSub(const BigInt *const, const BigInt *const, BigInt *);
extern BigInt* DoMul(const BigInt *const, const BigInt *const, BigInt *);
extern int GetMaxLeftShiftLen(const BigInt *const);
extern int GetMaxRightShiftLen(const BigInt *const);
extern int IsZero(const BigInt *const);
extern BigInt* DoDiv(const BigInt *const, const BigInt *const, BigInt *, BigInt *);
extern char* Add(const char *const, const char *const, char *);
extern char* Sub(const char *const, const char *const, char *);
extern char* Mul(const char *const, const char *const, char *);
extern char* Div(const char *const, const char *const, const char *const, char *);
extern int DoCompare(const BigInt *const, const BigInt *const);
extern int Compare(const char *const, const char *const); 
extern BigInt* DoMod(const BigInt *const, const BigInt *const, BigInt *);
extern char* Mod(const char *const, const char *const, char *);
extern BigInt* DoPow(const BigInt *const, const BigInt *const, BigInt *);
extern char* Pow(const char *const, const char *const, char *);
extern BigInt* DoPowMod(const BigInt *const, const BigInt *const, const BigInt *const, BigInt *);
extern char* PowMod(const char *const, const char *const, const char *const, char *);;
extern Number* StrToNumber(const char *const, Number *);
extern char* NumberToStr(const Number *const, char *);
extern BigInt* StrToBigInt(const char *const, BigInt *);
extern char* BigIntToStr(const BigInt *const, char *);
extern BigInt* byteSequenceToBinBigInt(const unsigned char *const, int, BigInt *);
extern void PrintBigInt(const BigInt *const);
extern void PrintBigIntTrueForm(const BigInt *const);
extern void PrintNumber(const Number *const);

#endif