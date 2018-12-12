#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../common.h"


#define INIT_STATE_POOL_SIZE 100

static unsigned char mask[AES_GROUP_SIZE];
static unsigned char init_state[AES_GROUP_SIZE];
static struct timeval cur_time;

static unsigned char init_state_pool[INIT_STATE_POOL_SIZE] = {
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

static void get_random(unsigned char *state, unsigned char *random, const int size)
{
	int i;
	unsigned char *p_time;

	/*  */
	p_time = &(cur_time.tv_sec);
	for(i = 0; i < sizeof(cur_time.tv_sec); i++)
	{
		mask[i] = p_time[i];
	}
	p_time = &(cur_time.tv_usec);
	for(i = 0; i < sizeof(cur_time.tv_usec); i++)
	{
		mask[sizeof(cur_time.tv_sec) + i] = p_time[i];
	}

	if(sizeof(cur_time.tv_sec) + i != AES_GROUP_SIZE)
	{
		printf("get_random, init mask err %d", sizeof(cur_time.tv_sec) + i);
		exit(1);
	}

	/*  */
	if(RANDOM_DEBUG)
	{
		printf("mask\n");
		for(i = 0; i < AES_GROUP_SIZE; i ++)
	  {
	      printf("%x ", mask[i]);
	  }
	  printf("\n\n");
	}

	/* generate mask sequence */
	aes_ofb_encrypt(mask, AES_GROUP_SIZE);

	/*  */
	for(i = 0; i < AES_GROUP_SIZE; i++)
	{
		state[i] ^= mask[i];
	}

	/* get random sequence */
	aes_cbc_encrypt(state, AES_GROUP_SIZE);
	for(i = 0; i < size; i++)
	{
		random[i] = state[i];
	}

	/*  */
	for(i = 0; i < AES_GROUP_SIZE; i++)
	{
		state[i] ^= mask[i];
	}

	/* init new state */
	aes_cfb_encrypt(state, AES_GROUP_SIZE);
}

void get_specified_size_random(unsigned char *random, const int size)
{
	int i, j;
	int state_start_index;

	/* get time */
	gettimeofday(&cur_time, NULL);

  /*  */
 	state_start_index = cur_time.tv_usec % (long)INIT_STATE_POOL_SIZE;
 	for(i = 0; i < AES_GROUP_SIZE; i++)
 	{
 		init_state[i] = init_state_pool[state_start_index++ % AES_GROUP_SIZE];
 	}

  /*  */
  if(RANDOM_DEBUG)
	{
  	printf("start to get random, size is %d\n\n", size);
  }

  /*  */
	for(i = 0; i < size; i += AES_GROUP_SIZE)
	{
		/*  */
		if(RANDOM_DEBUG)
		{
			printf("init_state\n");
			for(j = 0; j < AES_GROUP_SIZE; j ++)
		  {
		      printf("%x ", init_state[j]);
		  }
		  printf("\n\n");
		}
		

		if(i + AES_GROUP_SIZE > size)
		{
			get_random(init_state, &random[i], size - i);
		}
		else
		{
			get_random(init_state, &random[i], AES_GROUP_SIZE);
		}
	}
}

BigInt* DoGetPositiveRandBigInt(const int byteLen, BigInt *result)
{
    int i;
    memset(result->bit, 0, BIG_INT_BIT_LEN);
    
    /*  */
    unsigned char random_byte_sequence[byteLen];

    /*  */
    get_specified_size_random(random_byte_sequence, byteLen);

    if(RANDOM_DEBUG)
    {
        printf("hex odd random result begin\n");
        for(i = 0; i < byteLen; i++)
        {
            printf("%x ", random_byte_sequence[i]);
        }
        printf("\nhex odd random result end\n\n");
    }

    /*  */
    byteSequenceToBinBigInt(random_byte_sequence, byteLen, result);

    /* convert to positive integer */
    result->bit[SIGN_BIT] = 0;

    /*  */  
    return result;
}

BigInt* DoGetPositiveOddRandBigInt(const int if_fix_len, const int byteLen, BigInt* result)
{
    DoGetPositiveRandBigInt(byteLen, result);

    /* convert to odd integer */
    result->bit[0] = 1;
    
    /* fix min prime len */
    if(if_fix_len)
    {
        result->bit[byteLen * BYTE_SIZE - 1] = 1;
    }

    /*  */  
    return result;
}

BigInt* DoGetRand(const BigInt *const n, BigInt *result)
{
	 /*  */
  int i;
  BigInt tmp;

  /*  */
  memset(result->bit, 0, BIG_INT_BIT_LEN);

  /*  */
  int random_byte_sequence_size = floor(BIG_INT_BIT_LEN / BYTE_SIZE);
  
  
  /*  */
  unsigned char random_byte_sequence[random_byte_sequence_size];

  /*  */
  get_specified_size_random(random_byte_sequence, random_byte_sequence_size);

  if(RANDOM_DEBUG)
  {
      printf("hex random result begin\n");
      for(i = 0; i < random_byte_sequence_size; i ++)
      {
          printf("%x ", random_byte_sequence[i]);
      }
      printf("\nhex random result end\n\n");
  }

  /*  */
  byteSequenceToBinBigInt(random_byte_sequence, random_byte_sequence_size, &tmp);

  /*  */
  return DoMod(&tmp, n, result);
}

BigInt* DoGetPositiveRand(const BigInt *const n, BigInt *result)
{
	BigInt zero;

	StrToBigInt("0", &zero);

	DoGetRand(n, result);

	/* convert to positive integer */
  if(DoCompare(result, &zero) < 0)
  {
  	DoSub(&zero, result, result);
  }

  return result;
}