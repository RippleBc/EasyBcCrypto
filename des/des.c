#include "../common.h"

static unsigned char secrets[DES_ROUND_NUM][DES_SUB_GROUPING_SIZE] = {
	{0x01, 0x02, 0x03, 0x04},
	{0x02, 0x03, 0x04, 0x05},
	{0x03, 0x04, 0x05, 0x06},
	{0x04, 0x05, 0x06, 0x07},
	{0x05, 0x06, 0x07, 0x08},
	{0x09, 0x0a, 0x0b, 0x0c},
	{0x0a, 0x0b, 0x0c, 0x0d},
	{0x0b, 0x0c, 0x0d, 0x0e},
	{0x0c, 0x0d, 0x0e, 0x0f},
	{0x10, 0x11, 0x12, 0x13},
	{0x11, 0x12, 0x13, 0x14},
	{0x12, 0x13, 0x14, 0x15}
};

static unsigned char tmp[8];

void des_encrypt(unsigned char *source)
{
	int i;
	int j;

	/*  */
	i = 0;
	while(1)
	{
		for(j = 0; j < DES_GROUPING_SIZE; j++)
		{
			tmp[j] = source[j];
		}

		/* xor */
		for(j = 0; j < DES_SUB_GROUPING_SIZE; j++)
		{
			tmp[j] ^= secrets[i][j];
		}

		if(i < DES_ROUND_NUM - 1)
		{
			/* swap */
			for(j = 0; j < DES_GROUPING_SIZE; j++)
			{
				if(j < DES_SUB_GROUPING_SIZE)
				{
					source[j] = tmp[DES_SUB_GROUPING_SIZE + j];
				}
				else
				{
					source[j] = tmp[j - DES_SUB_GROUPING_SIZE];
				}
			}

			i++;
		}
		else
		{
			for(j = 0; j < DES_GROUPING_SIZE; j++)
			{
				source[j] = tmp[j];
			}

			/*  */
			break;
		}
	}
}


void des_decrypt(unsigned char *source)
{
	int i;
	int j;

	/*  */
	i = 0;
	while(1)
	{
		for(j = 0; j < DES_GROUPING_SIZE; j++)
		{
			tmp[j] = source[j];
		}

		/* xor */
		for(j = 0; j < DES_SUB_GROUPING_SIZE; j++)
		{
			tmp[j] ^= secrets[DES_ROUND_NUM - 1 - i][j];
		}

		if(i < DES_ROUND_NUM - 1)
		{
			/* swap */
			for(j = 0; j < DES_GROUPING_SIZE; j++)
			{
				if(j < DES_SUB_GROUPING_SIZE)
				{
					source[j] = tmp[DES_SUB_GROUPING_SIZE + j];
				}
				else
				{
					source[j] = tmp[j - DES_SUB_GROUPING_SIZE];
				}
			}

			i++;
		}
		else
		{
			for(j = 0; j < DES_GROUPING_SIZE; j++)
			{
				source[j] = tmp[j];
			}

			/*  */
			break;
		}
	}
}