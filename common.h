#define AES_GROUP_SIZE 16
#define AES_SUB_GROUP_SIZE 4

#define AES_ROUND_NUM 12

#define DES_GROUPING_SIZE 8
#define DES_SUB_GROUPING_SIZE (DES_GROUPING_SIZE / 2)

#define DES_ROUND_NUM 12

#define MAX_STR_SIZE 1024 * 10

#define AES_DEBUG (0)

#define RANDOM_DEBUG (1)
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

void get_specified_size_random(unsigned char *, unsigned char *, int);