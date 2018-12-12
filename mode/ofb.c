#include "../common.h"

static unsigned char tmp[AES_GROUP_SIZE];

// notice, at OFB mode, encrypt twice is same to decrypt
static void crypt(unsigned char *source, const int size)
{
	int i, j;

	for(j = 0; j < AES_GROUP_SIZE; j++)
	{
		tmp[j] = init_vector[j];
	}

	/*  */
	for(i = 0; i < size; i += AES_GROUP_SIZE)
	{

		aes_encrypt(tmp);

		for(j = 0; j < AES_GROUP_SIZE &&  i + j < size; j++)
		{
			source[i + j] ^= tmp[j];
		}
	}
}

void aes_ofb_encrypt(unsigned char *source, const int size)
{
	crypt(source, size);
}

void aes_ofb_decrypt(unsigned char *source, const int size)
{
	crypt(source, size);
}

// 0 ^ 1 = 1
// 0 ^ 0 = 0
// 1 ^ 0 = 1
// 1 ^ 1 = 0
