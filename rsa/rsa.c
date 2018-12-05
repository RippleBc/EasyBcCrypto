#include "../common.h"

#define MILLER_RABIN_TEST_TIMES 5

void DoGenerateRsaKey(int byteLen)
{
	printf("generating rsa key ...\n");

	BigInt p, q;
	int i;
	char s_result[MAX_STR_SIZE];

	DoGenRandomPrime(byteLen, MILLER_RABIN_TEST_TIMES, &p);

	BigIntToStr(&p, s_result);
	for(i = 0; i < strlen(s_result); i++)
	{
		printf("%c", s_result[i]);
	}
	printf("\n");


	DoGenRandomPrime(byteLen, MILLER_RABIN_TEST_TIMES, &q);  

	BigIntToStr(&q, s_result);
	for(i = 0; i < strlen(s_result); i++)
	{
		printf("%c", s_result[i]);
	}
	printf("\n");
}