#include "../common.h"

unsigned char init_vector[AES_GROUP_SIZE] = {
	0x0d, 0x1d, 0x2d, 0x3d, 
	0x08, 0x18, 0x28, 0x38, 
	0x6b, 0x7b, 0x8b, 0x9b, 
	0x56, 0x66, 0x76, 0x86
};

unsigned char tmp[AES_GROUP_SIZE];
unsigned char pre_encrypted_text[AES_GROUP_SIZE];

void aes_cbc_encrypt(unsigned char *source)
{
	int i, j;
	int source_size = strnlen(source, MAX_STR_SIZE);

	for(i = 0; i < source_size; i += AES_GROUP_SIZE)
	{
		if(i == 0)
		{
			for(j = 0; j < AES_GROUP_SIZE; j++)
			{
				source[i + j] ^= init_vector[j];
			}
		}
		else
		{
			for(j = 0; j < AES_GROUP_SIZE; j++)
			{
				source[i + j] ^= source[i + j - AES_GROUP_SIZE];
			}
		}

		aes_encrypt(&source[i]);
	}
}

void aes_cbc_decrypt(unsigned char *source)
{
	int i, j;
	int source_size = strnlen(source, MAX_STR_SIZE);

	for(i = 0; i < source_size; i += AES_GROUP_SIZE)
	{
		/*  */
		for(j = 0; j < AES_GROUP_SIZE; j++)
		{
			tmp[j] = source[i + j];
		}

		/*  */
		aes_decrypt(&source[i]);

		if(i == 0)
		{
			for(j = 0; j < AES_GROUP_SIZE; j++)
			{
				source[i + j] ^= init_vector[j];
			}
		}
		else
		{
			for(j = 0; j < AES_GROUP_SIZE; j++)
			{
				source[i + j] ^= pre_encrypted_text[j];
			}
		}

		/*  */
		for(j = 0; j < AES_GROUP_SIZE; j++)
		{
			pre_encrypted_text[j] = tmp[j];
		}
	}
}