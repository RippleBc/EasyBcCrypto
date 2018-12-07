#include "../common.h"
#include <stdio.h>

#define MILLER_RABIN_TEST_TIMES 5

// #define STACK_SIZE 1024
// static stack_index = STACK_SIZE;
// static BigInt stack[STACK_SIZE];
// static void push_stack(BigInt *bi)
// {
// 	if(GENE_D_DEBUG)
// 	{
// 		unsigned char dest[MAX_STR_SIZE];
// 		BigIntToStr(bi, dest);
// 		printf("push: ");
// 		for(int i = 0; i < strlen(dest); i++)
// 		{
// 			printf("%c", dest[i]);
// 		}
// 		printf("\n");
// 	}


// 	CopyBigInt(bi, &stack[--stack_index]);
// }
// static void pop_stack(BigInt *bi)
// {
// 	if(stack_index >= STACK_SIZE)
// 	{
// 		StrToBigInt("0", bi);
// 		return;
// 	}
// 	CopyBigInt(&stack[stack_index++], bi);

// 	if(GENE_D_DEBUG)
// 	{
// 		unsigned char dest[MAX_STR_SIZE];
// 	 	BigIntToStr(bi, dest);
// 	 	printf("pop: ");
// 	 	for(int i = 0; i < strlen(dest); i++)
// 	 	{
// 	 		printf("%c", dest[i]);
// 	 	}
// 	 	printf("\n");
// 	}
// }

// BigInt *GeneD(BigInt *e, BigInt *d, BigInt *l)
// {
// 	/* e * d - l * k = 1 */
// 	BigInt t_e, t_l, k, one, zero, t_a, remainder;

// 	StrToBigInt("1", &one);
// 	StrToBigInt("0", &zero);

// 	CopyBigInt(e, &t_e);
// 	CopyBigInt(l, &t_l);

// 	push_stack(&t_l);
// 	push_stack(&t_e);
	

// 	int i = 0;
// 	while(DoCompare(&t_e, &one) != 0 && DoCompare(&t_l, &one) != 0)
// 	{
// 		if(i % 2 == 0)
// 		{
// 			DoMod(&t_l, &t_e, &t_l);
// 		}
// 		else
// 		{
// 			DoMod(&t_e, &t_l, &t_e);
// 		}

// 		push_stack(&t_l);
// 		push_stack(&t_e);

// 		i++;
// 	}

// 	pop_stack(&t_e);
// 	pop_stack(&t_l);

// 	pop_stack(&t_e);
// 	pop_stack(&t_l);

// 	StrToBigInt("1", e);
// 	StrToBigInt("1", d);
// 	while(1)
// 	{
// 		if(i % 2 == 0)
// 		{
// 			DoMul(&t_e, d, &t_a);
// 			DoSub(&t_a, &one, &t_a);
// 			DoDiv(&t_a, &t_l, &k, &remainder);

// 			if(GENE_D_DEBUG)
// 			{
// 				unsigned char dest[MAX_STR_SIZE];
// 			 	BigIntToStr(&k, dest);
// 			 	printf("k: ");
// 			 	for(int i = 0; i < strlen(dest); i++)
// 			 	{
// 			 		printf("%c", dest[i]);
// 			 	}
// 			 	printf("\n");
// 			}
// 		}
// 		else
// 		{
// 			DoMul(&t_l, &k, &t_a);
// 			DoAdd(&t_a, &one, &t_a);
// 			DoDiv(&t_a, &t_e, d, &remainder);

// 			if(GENE_D_DEBUG)
// 			{
// 				unsigned char dest[MAX_STR_SIZE];
// 			 	BigIntToStr(d, dest);
// 			 	printf("d: ");
// 			 	for(int i = 0; i < strlen(dest); i++)
// 			 	{
// 			 		printf("%c", dest[i]);
// 			 	}
// 			 	printf("\n");
// 			}
// 		}

// 		pop_stack(&t_e);
// 		if(DoCompare(&t_e, &zero) == 0)
// 		{
// 			break;
// 		}
// 		pop_stack(&t_l);

// 		i++;
// 	}

// 	return d;
// }

BigInt *GeneD(BigInt *e, BigInt *d, BigInt *l)
{
	BigInt k, result;
	DoExGcd(l, e, &k, d, &result);

	return d;
}

void DoGenerateRsaKey(int byteLen, char *key_pair_file)
{
	printf("generating rsa key ...\n");

	int i;
	BigInt p, q, e, d, n, pMinusOne, qMinusOne, l, gcd;
	BigInt one;
	char s_result[MAX_STR_SIZE], s_e[MAX_STR_SIZE], s_d[MAX_STR_SIZE], s_n[MAX_STR_SIZE];

	/*  */
	StrToBigInt("1", &one);

	DoGenRandomPrime(byteLen, MILLER_RABIN_TEST_TIMES, &p);	

	do
	{
		DoGenRandomPrime(byteLen, MILLER_RABIN_TEST_TIMES, &q);
	}
	while(DoCompare(&p, &q) == 0);

	/* generate n */
	DoMul(&p, &q, &n);
	BigIntToStr(&n, s_n);

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
		for(i = 0; i < strlen(s_n); i++)
		{
			printf("%c", s_n[i]);
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
 		/*  */
 		do
 		{
 			DoGetPositiveRand(&l, &e);
 		} while(DoCompare(&e, &one) <= 0);

 		/*  */
 		DoGcd(&e, &l, &gcd);
 	}
 	while(DoCompare(&gcd, &one) != 0);
	BigIntToStr(&e, s_e);

 	/* generate d */
 	GeneD(&e, &d, &l);
 	BigIntToStr(&d, s_d);

 	if(RSA_DEBUG)
	{
		printf("e: ");
		for(i = 0; i < strlen(s_e); i++)
		{
			printf("%c", s_e[i]);
		}
		printf("\n");

		printf("d: ");
		for(i = 0; i < strlen(s_d); i++)
		{
			printf("%c", s_d[i]);
		}
		printf("\n");
	}

	/* generate key pair file */
	char private_file_name[FILE_NAME_LEN];
	snprintf(private_file_name, FILE_NAME_LEN, "%s_private.rsa",  key_pair_file);
	FILE *p_private_file;
	p_private_file = fopen(private_file_name, "wt");
	if(p_private_file == NULL)
	{
		printf("DoGenerateRsaKey, open file %s err\n", private_file_name);
		exit(1);
	}

	char public_file_name[FILE_NAME_LEN];
	snprintf(public_file_name, FILE_NAME_LEN, "%s_public.rsa",  key_pair_file);
	FILE *p_public_file;
	p_public_file = fopen(public_file_name, "wt");
	if(p_public_file == NULL)
	{
		printf("DoGenerateRsaKey, open file %s err\n", public_file_name);
		exit(1);
	}

	if(EOF == fputs(s_e, p_public_file) || EOF == fputc('\n', p_public_file) 
		|| EOF == fputs(s_d, p_private_file) || EOF == fputc('\n', p_private_file) 
		|| EOF == fputs(s_n, p_public_file) || EOF == fputs(s_n, p_private_file))
	{
		printf("DoGenerateRsaKey, write e to file err\n");
		exit(1);
	}

	fclose(p_private_file);
	fclose(p_public_file);
}

static char *Crypt(unsigned char *source, unsigned char *dest, BigInt *key, BigInt *n)
{
	BigInt s, d;

	StrToBigInt(source, &s);

	DoPowMod(&s, key, n, &d);

	return BigIntToStr(&d, dest);
}

char *RsaEncrypt(unsigned char *source, unsigned char *dest, char *key_pair_file)
{
	/*  */
	char public_file_name[FILE_NAME_LEN];
	snprintf(public_file_name, FILE_NAME_LEN, "%s_public.rsa",  key_pair_file);
	FILE *p_public_file;
	p_public_file = fopen(public_file_name, "rt");
	if(p_public_file == NULL)
	{
		printf("RsaEncrypt, open file %s err\n", public_file_name);
		exit(1);
	}

	/*  */
	char buffer[BIG_INT_BIT_LEN];
	BigInt e, n;
	char c;
	int mark = 0;
	while((fgets(buffer, BIG_INT_BIT_LEN, p_public_file)) != NULL)
	{
		if(mark == 0)
		{
			StrToBigInt(buffer, &e);
		}
		else
		{
			StrToBigInt(buffer, &n);
		}
	}
	if(!feof(p_public_file))
	{
		printf("RsaEncrypt, fgets err %s\n", public_file_name);
		exit(1);
	}
	fclose(p_public_file);

	return Crypt(source, dest, &e, &n);
}

char *RsaDecrypt(unsigned char *source, unsigned char *dest, char *key_pair_file)
{
	/*  */
	char private_file_name[FILE_NAME_LEN];
	snprintf(private_file_name, FILE_NAME_LEN, "%s_private.rsa",  key_pair_file);
	FILE *p_private_file;
	p_private_file = fopen(private_file_name, "rt");
	if(p_private_file == NULL)
	{
		printf("RsaEncrypt, open file %s err\n", private_file_name);
		exit(1);
	}

	/*  */
	char buffer[BIG_INT_BIT_LEN];
	BigInt d, n;
	char c;
	int mark = 0;
	while((fgets(buffer, BIG_INT_BIT_LEN, p_private_file)) != NULL)
	{
		if(mark == 0)
		{
			StrToBigInt(buffer, &d);
		}
		else
		{
			StrToBigInt(buffer, &n);
		}
	}
	if(!feof(p_private_file))
	{
		printf("RsaEncrypt, fgets err %s\n", private_file_name);
		exit(1);
	}
	fclose(p_private_file);

	return Crypt(source, dest, &d, &n);
}