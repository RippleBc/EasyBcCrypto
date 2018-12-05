#include "../common.h"

#define MILLER_RABIN_TEST_TIMES 5

void DoGenerateRsaKey(BigInt *e, BigInt *d, BigInt *n, int byteLen)
{
	printf("generating rsa key ...\n");

	int i;
	BigInt p, q, gcd, one, pMinusOne, qMinusOne, l;
	char s_result[MAX_STR_SIZE];

	/*  */
	StrToBigInt("1", &one);

	DoGenRandomPrime(byteLen, MILLER_RABIN_TEST_TIMES, &p);	

	do
	{
		DoGenRandomPrime(byteLen, MILLER_RABIN_TEST_TIMES, &q);
	}
	while(DoCompare(&p, &q) == 0);

	/* generate n */
	DoMul(&p, &q, n);

	/* generate l */
	DoSub(&p, &one, &pMinusOne);
	DoSub(&q, &one, &qMinusOne);
	DoLcm(&pMinusOne, &qMinusOne, &l);
 	
	if(RSA_DEBUG)
	{
		printf("p: ");
		BigIntToStr(&p, s_result);
		for(i = 0; i < strlen(s_result); i++)
		{
			printf("%c", s_result[i]);
		}
		printf("\n");

		printf("q: ");
		BigIntToStr(&q, s_result);
		for(i = 0; i < strlen(s_result); i++)
		{
			printf("%c", s_result[i]);
		}
		printf("\n");

		printf("n: ");
		BigIntToStr(n, s_result);
		for(i = 0; i < strlen(s_result); i++)
		{
			printf("%c", s_result[i]);
		}
		printf("\n");

		printf("l: ");
		BigIntToStr(&l, s_result);
		for(i = 0; i < strlen(s_result); i++)
		{
			printf("%c", s_result[i]);
		}
		printf("\n");
	}

 	/* generate e */
 	do{
 		DoGetRand(&l, e);

 		DoGcd(e, &l, &gcd);
 	}
 	while(DoCompare(&gcd, &one) != 0);
	
 	/* generate d */
 	do{
 		DoGetRand(&l, d);

 		DoGcd(d, &l, &gcd);
 	}
 	while(DoCompare(&gcd, &one) != 0 || DoCompare(e, d) == 0);


 	if(RSA_DEBUG)
	{
		printf("e: ");
		BigIntToStr(e, s_result);
		for(i = 0; i < strlen(s_result); i++)
		{
			printf("%c", s_result[i]);
		}
		printf("\n");

		printf("d: ");
		BigIntToStr(d, s_result);
		for(i = 0; i < strlen(s_result); i++)
		{
			printf("%c", s_result[i]);
		}
		printf("\n");
	}
}


void RsaEncrypt(char *source, char *dest, int size, BigInt *key, BigInt *n)
{
	int i;
	BigInt s, d;
	for(i = 0; i < size; i++)
	{
		// StrToBigInt()
		// source[i]
	}
}

void RsaDecrypt(char *source, char *dest, int size, BigInt *key, BigInt *n)
{

}
