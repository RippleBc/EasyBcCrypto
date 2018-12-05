#include "../common.h"

#define MILLER_RABIN_TEST_TIMES 5

#define STACK_SIZE 1024

static stack_index = STACK_SIZE;
static BigInt stack[STACK_SIZE];
static void push_stack(BigInt *bi)
{
	if(GENE_D_DEBUG)
	{
		unsigned char dest[MAX_STR_SIZE];
		BigIntToStr(bi, dest);
		printf("push: ");
		for(int i = 0; i < strlen(dest); i++)
		{
			printf("%c", dest[i]);
		}
		printf("\n");
	}


	CopyBigInt(bi, &stack[--stack_index]);
}
static void pop_stack(BigInt *bi)
{
	if(stack_index >= STACK_SIZE)
	{
		StrToBigInt("0", bi);
		return;
	}
	CopyBigInt(&stack[stack_index++], bi);

	if(GENE_D_DEBUG)
	{
		unsigned char dest[MAX_STR_SIZE];
	 	BigIntToStr(bi, dest);
	 	printf("pop: ");
	 	for(int i = 0; i < strlen(dest); i++)
	 	{
	 		printf("%c", dest[i]);
	 	}
	 	printf("\n");
	}
}

BigInt *GeneD(BigInt *e, BigInt *d, BigInt *l)
{
	/* e * d - l * k = 1 */
	BigInt t_e, t_l, k, one, zero, t_a, remainder;

	StrToBigInt("1", &one);
	StrToBigInt("0", &zero);

	CopyBigInt(e, &t_e);
	CopyBigInt(l, &t_l);

	push_stack(&t_l);
	push_stack(&t_e);
	

	int i = 0;
	while(DoCompare(&t_e, &one) != 0)
	{
		if(i % 2 == 0)
		{
			DoMod(&t_l, &t_e, &t_l);
		}
		else
		{
			DoMod(&t_e, &t_l, &t_e);
		}

		push_stack(&t_l);
		push_stack(&t_e);

		i++;
	}

	pop_stack(&t_e);
	pop_stack(&t_l);

	pop_stack(&t_e);
	pop_stack(&t_l);

	StrToBigInt("1", d);

	i = 0;
	while(1)
	{
		if(i % 2 == 0)
		{
			DoMul(&t_e, d, &t_a);
			DoSub(&t_a, &one, &t_a);
			DoDiv(&t_a, &t_l, &k, &remainder);

			if(GENE_D_DEBUG)
			{
				unsigned char dest[MAX_STR_SIZE];
			 	BigIntToStr(&k, dest);
			 	printf("k: ");
			 	for(int i = 0; i < strlen(dest); i++)
			 	{
			 		printf("%c", dest[i]);
			 	}
			 	printf("\n");
			}
		}
		else
		{
			DoMul(&t_l, &k, &t_a);
			DoAdd(&t_a, &one, &t_a);
			DoDiv(&t_a, &t_e, d, &remainder);

			if(GENE_D_DEBUG)
			{
				unsigned char dest[MAX_STR_SIZE];
			 	BigIntToStr(d, dest);
			 	printf("d: ");
			 	for(int i = 0; i < strlen(dest); i++)
			 	{
			 		printf("%c", dest[i]);
			 	}
			 	printf("\n");
			}
		}

		pop_stack(&t_e);
		if(DoCompare(&t_e, &zero) == 0)
		{
			break;
		}
		pop_stack(&t_l);

		i++;
	}

	return d;
}

void DoGenerateRsaKey(BigInt *e, BigInt *d, BigInt *n, int byteLen)
{
	printf("generating rsa key ...\n");

	int i;
	BigInt p, q, gcd, one, pMinusOne, qMinusOne, l, t_a, t_b;
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
 	GeneD(e, d, &l);

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

static char *Crypt(unsigned char *source, unsigned char *dest, BigInt *key, BigInt *n)
{
	BigInt s, d;

	StrToBigInt(source, &s);

	DoPowMod(&s, key, n, &d);

	return BigIntToStr(&d, dest);
}

char *RsaEncrypt(unsigned char *source, unsigned char *dest, BigInt *key, BigInt *n)
{
	return Crypt(source, dest, key, n);
}

char *RsaDecrypt(unsigned char *source, unsigned char *dest, BigInt *key, BigInt *n)
{
	return Crypt(source, dest, key, n);
}
