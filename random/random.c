#include <time.h>
#include "../common.h"

static unsigned char mask[AES_GROUP_SIZE];

static void get_random(unsigned char *state, unsigned char *random, int size)
{
	int i, j;

	/* time */
	int time_t_size = sizeof(time_t);
	unsigned char *p_time;

	/* get time */
	time_t l_time = time((time_t *)NULL);
	p_time = &l_time;

	/* generate mask sequence */
	for(i = 0; i < AES_GROUP_SIZE; i++)
	{
		mask[i] = p_time[i % time_t_size];
	}
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

void get_specified_size_random(unsigned char *state, unsigned char *random, int size)
{
	int i, j;
	for(i = 0; i < size; i += AES_GROUP_SIZE)
	{
		/*  */
		if(RANDOM_DEBUG)
		{
			printf("state\n");
			for(j = 0; j < AES_GROUP_SIZE; j ++)
		  {
		      printf("%x ", state[j]);
		  }
		  printf("\n\n");
		}
		

		if(i + AES_GROUP_SIZE > size)
		{
			get_random(state, &random[i], size - i);
		}
		else
		{
			get_random(state, &random[i], AES_GROUP_SIZE);
		}
	}
}