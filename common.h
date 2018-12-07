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

#define AES_DEBUG (0)
#define RANDOM_DEBUG (0)
#define MILLER_RABIN_DEBUG (0)
#define DO_POW_MOD_DEBUG (0)
#define RSA_DEBUG (1)
#define GENE_D_DEBUG (1)

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

extern char* ChangeStringRadix(char *, int, int, char *);
extern BigInt* CopyBigInt(BigInt  *, BigInt  *);
extern BigInt* ShiftArithmeticLeft(BigInt *, int indent, BigInt *);
extern BigInt* ShiftArithmeticRight(BigInt* , int indent, BigInt *);
extern BigInt* DoAdd(BigInt *, BigInt *, BigInt *);
extern BigInt* DoSub(BigInt *, BigInt *, BigInt *);
extern BigInt* DoMul(BigInt *, BigInt *, BigInt *);
extern int GetMaxLeftShiftLen(BigInt *);
extern int GetMaxRightShiftLen(BigInt *);
extern int IsZero(BigInt *);
extern BigInt* DoDiv(BigInt *, BigInt *, BigInt *, BigInt *);
extern char* Add(char *, char *, char *);
extern char* Sub(char *, char *, char *);
extern char* Mul(char *, char *, char *);
extern char* Div(char *, char *, char *, char *);
extern int DoCompare(BigInt *, BigInt *);
extern int Compare(char *, char *); 
extern BigInt* DoMod(BigInt *, BigInt *, BigInt *);
extern char* Mod(char *, char *, char *);
extern BigInt* DoPow(BigInt *, BigInt *, BigInt *);
extern char* Pow(char *, char *, char *);
extern BigInt* DoPowMod(BigInt *, BigInt *, BigInt *, BigInt *);
extern char* PowMod(char *, char *, char *, char *);;
extern Number* StrToNumber(char *, Number *);
extern char* NumberToStr(Number *, char *);
extern BigInt* StrToBigInt(char *, BigInt *);
extern char* BigIntToStr(BigInt *, char *);
extern BigInt* byteSequenceToBinBigInt(unsigned char *, int, BigInt *);
extern void PrintBigInt(BigInt*);
extern void PrintBigIntTrueForm(BigInt *);
extern void PrintNumber(Number *);