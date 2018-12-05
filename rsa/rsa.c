#include "../common.h"

void DoGenerateRsaKey()
{
	printf("generating rsa key ...\n");

	BigInt p, q;
	int i;
	char s_result[MAX_STR_SIZE];

	DoGenRandomPrime(16, 5, &p);

	BigIntToStr(&p, s_result);
	for(i = 0; i < strlen(s_result); i++)
	{
		printf("%c", s_result[i]);
	}
	printf("\n");


	DoGenRandomPrime(16, 5, &q);  

	BigIntToStr(&q, s_result);
	for(i = 0; i < strlen(s_result); i++)
	{
		printf("%c", s_result[i]);
	}
	printf("\n");
}