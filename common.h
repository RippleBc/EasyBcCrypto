#define AES_GROUP_SIZE 16
#define AES_SUB_GROUP_SIZE 4

#define AES_ROUND_NUM 10

#define DES_GROUPING_SIZE 8
#define DES_SUB_GROUPING_SIZE (DES_GROUPING_SIZE / 2)

#define DES_ROUND_NUM 12

#define MAX_STR_SIZE 1024 * 10

extern void aes_encrypt(unsigned char *);
extern void aes_decrypt(unsigned char *);

extern void des_encrypt(unsigned char *);
extern void des_decrypt(unsigned char *);

extern unsigned char init_vector[];

extern void aes_cbc_encrypt(unsigned char *);
extern void aes_cbc_decrypt(unsigned char *);

extern void aes_cfb_encrypt(unsigned char *);
extern void aes_cfb_decrypt(unsigned char *);