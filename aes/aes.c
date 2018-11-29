#include "../common.h"

#define DICT_SIZE 256

static unsigned char compare[DICT_SIZE];

static unsigned char dictionaries[DICT_SIZE] = {
	0x70, 0x10, 0x20, 0xf0, 0x40, 0xb0, 0x60, 0x00, 0x80, 0x90, 0xa0, 0x50, 0xc0, 0xd0, 0xe0, 0x30,
	0xb4, 0x91, 0x21, 0x59, 0xf1, 0x51, 0x61, 0x71, 0x81, 0x11, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0x41,
	0x02, 0x12, 0x22, 0x32, 0x7b, 0x52, 0x62, 0x72, 0x82, 0x0d, 0xa2, 0xb2, 0xc2, 0xd2, 0xe2, 0xbb,
	0x95, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xa3, 0x78, 0xc3, 0xd3, 0xe3, 0xf3,
	0x04, 0x14, 0x24, 0x86, 0x44, 0x54, 0x64, 0x74, 0x08, 0x94, 0xa4, 0x01, 0xc4, 0xd4, 0xe4, 0xf4,
	0x9b, 0x15, 0x25, 0x35, 0x45, 0x2e, 0x0a, 0xfd, 0x3c, 0x03, 0xa5, 0xb5, 0xc5, 0xd5, 0xe5, 0xf5,
	0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76, 0x34, 0x96, 0xf6, 0xb6, 0xc6, 0xd6, 0xe6, 0xa6,
	0x07, 0x17, 0x27, 0x97, 0x47, 0x57, 0xbd, 0x77, 0x87, 0x6e, 0xa7, 0xb7, 0xea, 0xfb, 0xe7, 0xf7,
	0x09, 0x18, 0x28, 0x9f, 0x48, 0x58, 0x68, 0xb3, 0x88, 0x98, 0xa8, 0xb8, 0xc8, 0xd8, 0xe8, 0xf8,
	0x84, 0x19, 0x29, 0x39, 0x49, 0x31, 0x69, 0x79, 0x89, 0x99, 0x0c, 0xb9, 0xc9, 0xd9, 0xdb, 0xf9,
	0x65, 0x1a, 0x2a, 0x3a, 0xca, 0x5a, 0x6a, 0x7a, 0x8a, 0x9a, 0xaa, 0xba, 0x4a, 0xda, 0xc7, 0xfa,
	0x0b, 0x1b, 0x1e, 0xab, 0x4b, 0x5b, 0x6b, 0x42, 0x8b, 0x05, 0x3b, 0xf2, 0xcb, 0xe9, 0xeb, 0xd7,
	0xa9, 0x1c, 0x2c, 0x85, 0x4c, 0x5c, 0x6c, 0x7c, 0x8c, 0x9c, 0xac, 0xbc, 0xcc, 0xdc, 0xec, 0xfc,
	0x92, 0x1d, 0x2d, 0x3d, 0x4d, 0x5d, 0x6d, 0x7d, 0x8d, 0x9d, 0xad, 0x67, 0xcd, 0xdd, 0xed, 0x75,
	0x0e, 0x2b, 0x55, 0x3e, 0x4e, 0x5e, 0x37, 0x7e, 0x8e, 0x9e, 0xae, 0xbe, 0xce, 0xde, 0xee, 0xfe,
	0x0f, 0x1f, 0x2f, 0x3f, 0x6f, 0x5f, 0x4f, 0x7f, 0x8f, 0x38, 0xaf, 0xbf, 0xcf, 0xdf, 0xef, 0xff
};

static unsigned char secret[AES_GROUP_SIZE] = {
	0x0d, 0x1d, 0x2d, 0x3d, 
	0x08, 0x18, 0x28, 0x38, 
	0x6b, 0x7b, 0x8b, 0x9b, 
	0x56, 0x66, 0x76, 0x86
};

static unsigned char column_secret[AES_GROUP_SIZE] = {
	0x58, 0x68, 0x78, 0x88,
	0x32, 0x41, 0x36, 0x81,
	0xcd, 0xab, 0x76, 0x56,
	0x43, 0x98, 0x7d, 0xc5
};

static unsigned char tmp[AES_GROUP_SIZE];

void aes_encrypt(unsigned char *source)
{
	int i;
	int j;
	unsigned char index;
	int round_index;
	
	/*  */
	memset(compare, 0, DICT_SIZE);
	for(int i = 0; i < DICT_SIZE; i++)
	{
		if(compare[dictionaries[i]] != 0)
		{
			printf("invalid dict\n");
			exit(1);
		}
		compare[dictionaries[i]] = 1;
	}

	if(AES_DEBUG)
	{
		printf("aes_encrypt begin\norigin text:\n");
    for(i = 0; i < AES_GROUP_SIZE; i++)
    {
        if(i % 4 == 0)
        {
            printf("\n");
        }
        printf("%x ", source[i]);
    }
    printf("\n\n");
	}


	for(round_index = 0; round_index < AES_ROUND_NUM; round_index++)
	{
		/* Encrypt SubBytes	*/
		for(i = 0; i < AES_GROUP_SIZE; i++)
		{
			index = source[i];
			source[i] = dictionaries[index];
		}

		if(AES_DEBUG)
		{
			printf("Encrypt SubBytes\n");
			for(i = 0; i < AES_GROUP_SIZE; i++)
		  {
		  	if(i % 4 == 0)
		  	{
		  		printf("\n");
		  	}
		  	printf("%x ", source[i]);
		  }
		  printf("\n\n");
		}

		/* Encrypt ShiftRows */
		for(i = 0; i < AES_SUB_GROUP_SIZE; i++)
		{
			tmp[i] = source[(i + 1) % AES_SUB_GROUP_SIZE];
		}
		for(i = 0; i < AES_SUB_GROUP_SIZE; i++)
		{
			source[i] = tmp[i];
		}
		for(i = 0; i < 2; i++)
		{
			for(j = 0; j < AES_SUB_GROUP_SIZE; j++)
			{
				tmp[j] = source[AES_SUB_GROUP_SIZE + (j + 1) % AES_SUB_GROUP_SIZE];
			}

			for(j = 0; j < AES_SUB_GROUP_SIZE; j++)
			{
				source[AES_SUB_GROUP_SIZE + j] = tmp[j];
			}
		}
		for(i = 0; i < 3; i++)
		{
			for(j = 0; j < AES_SUB_GROUP_SIZE; j++)
				{
				tmp[j] = source[AES_SUB_GROUP_SIZE * 2 + (j + 1) % AES_SUB_GROUP_SIZE];
			}

			for(j = 0; j < AES_SUB_GROUP_SIZE; j++)
			{
				source[2 * AES_SUB_GROUP_SIZE + j] = tmp[j];
			}
		}

		if(AES_DEBUG)
		{
			printf("Encrypt ShiftRows\n");
			for(i = 0; i < AES_GROUP_SIZE; i++)
		  {
		  	if(i % 4 == 0)
		  	{
		  		printf("\n");
		  	}
		  	printf("%x ", source[i]);
		  }
		  printf("\n\n");
		}

		/* Encrypt MixColumns */
		for(i = 0; i < AES_GROUP_SIZE; i++)
		{
			source[i] ^= column_secret[i];
		}

		if(AES_DEBUG)
		{
			printf("Encrypt MixColumns\n");
			for(i = 0; i < AES_GROUP_SIZE; i++)
		  {
		  	if(i % 4 == 0)
		  	{
		  		printf("\n");
		  	}
		  	printf("%x ", source[i]);
		  }
		  printf("\n\n");
		}

		/* Encrypt AddRoundKey */
		for(i = 0; i < AES_GROUP_SIZE; i ++)
		{
			source[i] ^= secret[i];
		}

		if(AES_DEBUG)
		{
			printf("Encrypt AddRoundKey\n");
			for(i = 0; i < AES_GROUP_SIZE; i++)
		  {
		  	if(i % 4 == 0)
		  	{
		  		printf("\n");
		  	}
		  	printf("%x ", source[i]);
		  }
		  printf("\n\n");
		}
	}
}

void aes_decrypt(unsigned char *source)
{
	int i;
	int j;
	int index;
	int round_index;

	if(AES_DEBUG)
	{
		printf("aes_decrypt begin\norigin text:\n");
    for(i = 0; i < AES_GROUP_SIZE; i++)
    {
        if(i % 4 == 0)
        {
            printf("\n");
        }
        printf("%x ", source[i]);
    }
    printf("\n\n");
	}

	for(round_index = 0; round_index < AES_ROUND_NUM; round_index++)
	{
		/* Decrypt AddRoundKey */
		for(i = 0; i < AES_GROUP_SIZE; i ++)
		{
			source[i] ^= secret[i];
		}

		if(AES_DEBUG)
		{
			printf("Decrypt AddRoundKey\n");
			for(i = 0; i < AES_GROUP_SIZE; i++)
		  {
		  	if(i % 4 == 0)
		  	{
		  		printf("\n");
		  	}
		  	printf("%x ", source[i]);
		  }
		  printf("\n\n");
		}

		/* Decrypt MixColumns */
		for(i = 0; i < AES_GROUP_SIZE; i++)
		{
			source[i] ^= column_secret[i];
		}

		if(AES_DEBUG)
		{
			printf("Decrypt MixColumns\n");
			for(i = 0; i < AES_GROUP_SIZE; i++)
		  {
		  	if(i % 4 == 0)
		  	{
		  		printf("\n");
		  	}
		  	printf("%x ", source[i]);
		  }
		  printf("\n\n");
		}

		/* Decrypt InShiftRows */
		for(i = 0; i < AES_SUB_GROUP_SIZE; i++)
		{
			tmp[(i + 1) % AES_SUB_GROUP_SIZE] = source[i];
		}
		for(i = 0; i < AES_SUB_GROUP_SIZE; i++)
		{
			source[i] = tmp[i];
		}
		for(i = 0; i < 2; i++)
		{
			for(j = 0; j < AES_SUB_GROUP_SIZE; j++)
			{
				tmp[(j + 1) % AES_SUB_GROUP_SIZE] = source[AES_SUB_GROUP_SIZE + j];
			}

			for(j = 0; j < AES_SUB_GROUP_SIZE; j++)
			{
				source[AES_SUB_GROUP_SIZE + j] = tmp[j];
			}
		}
		for(i = 0; i < 3; i++)
		{
			for(j = 0; j < AES_SUB_GROUP_SIZE; j++)
			{
				tmp[(j + 1) % AES_SUB_GROUP_SIZE] = source[AES_SUB_GROUP_SIZE * 2 + j];
			}

			for(j = 0; j < AES_SUB_GROUP_SIZE; j++)
			{
				source[AES_SUB_GROUP_SIZE * 2 + j] = tmp[j];
			}
		}

		if(AES_DEBUG)
		{
			printf("Decrypt InShiftRows\n");
			for(i = 0; i < AES_GROUP_SIZE; i++)
		  {
		  	if(i % 4 == 0)
		  	{
		  		printf("\n");
		  	}
		  	printf("%x ", source[i]);
		  }
		  printf("\n\n");
		}

		/* Decrypt InSubBytes */
		for(i = 0; i < AES_GROUP_SIZE; i ++)
		{
			for(j = 0; j < 256; j++)
			{
				if(dictionaries[j] == source[i])
				{
					source[i] = (unsigned char)j;
					break;
				}
			}
		}

		if(AES_DEBUG)
		{
			printf("Decrypt InSubBytes\n");
			for(i = 0; i < AES_GROUP_SIZE; i++)
		  {
		  	if(i % 4 == 0)
		  	{
		  		printf("\n");
		  	}
		  	printf("%x ", source[i]);
		  }
		  printf("\n\n");
		}
	}
}