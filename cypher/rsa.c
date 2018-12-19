#include "../common.h"
#include <stdio.h>

void GeneD(const mpz_t const e, mpz_t d, const mpz_t const l)
{
	mpz_t tmp;

	mpz_init(tmp);


		/* compute d */
		mpz_gcdext(G_BIG_INT_ONE, tmp, d, l, e);

		if(RSA_DEBUG)
		{
			gmp_printf("GeneD, s: %Zd\n", tmp);
			gmp_printf("GeneD, d: %Zd\n", d);
		}
}

void DoGenerateRsaKey(const int byteLen, const char *const key_pair_file)
{
	printf("DoGenerateRsaKey, generating rsa key ...\n");

	int fix_len = 1;
	mpz_t p, q, e, d, n, pMinusOne, qMinusOne, l, gcd;

	mpz_init(p);
	mpz_init(q);
	mpz_init(e);
	mpz_init(d);
	mpz_init(n);
	mpz_init(pMinusOne);
	mpz_init(qMinusOne);
	mpz_init(l);
	mpz_init(gcd);

	/* generate prime p and q */
	DoGenRandomPrime(fix_len, byteLen, &p);	
	do
	{
		DoGenRandomPrime(fix_len, byteLen, &q);
	}
	while(mpz_cmp(p, q) == 0);

	/* generate n */
	mpz_mul(&n, &p, &q);

	/* generate l */
	mpz_sub(pMinusOne, p, G_BIG_INT_ONE);
	mpz_sub(qMinusOne, q, G_BIG_INT_ONE);
	mpz_lcm(l, pMinusOne, qMinusOne);
 	
	if(RSA_DEBUG)
	{
		gmp_printf("DoGenerateRsaKey, p: %Zd\n", p);
		gmp_printf("DoGenerateRsaKey, q: %Zd\n", q);
		gmp_printf("DoGenerateRsaKey, n: %Zd\n", n);
		gmp_printf("DoGenerateRsaKey, l: %Zd\n", l);
	}
	while(1)
	{
		/* init e */
		DoGetPositiveRand(l, e);
		if(RSA_DEBUG)
		{
			gmp_printf("DoGenerateRsaKey, test e: %Zd\n", e);
		}
 		if(mpz_cmp(e, G_BIG_INT_TWO) <= 0)
 		{
 			continue;
 		}

 		/* check e */
 		mpz_gcd(gcd, e, l);
		if(mpz_cmp(gcd, G_BIG_INT_ONE) != 0)
		{
			continue;
		}
	 	if(RSA_DEBUG)
		{
			gmp_printf("DoGenerateRsaKey, e: %Zd\n", e);
		}

	 	/* generate d */
	 	GeneD(e, d, l);
	 	if(RSA_DEBUG)
		{
			gmp_printf("DoGenerateRsaKey, test d: %Zd\n", d);
		}
	 	if(mpz_cmp(d, G_BIG_INT_TWO) <= 0)
	 	{
	 		continue;
	 	}
	 	if(RSA_DEBUG)
		{
			gmp_printf("DoGenerateRsaKey, d: %Zd\n", d);
		}

		break;
	}

	/* generate key pair file */
	char private_file_name[FILE_NAME_LEN];
	snprintf(private_file_name, FILE_NAME_LEN, "keys/%s_private.rsa",  key_pair_file);
	FILE *p_private_file;
	p_private_file = fopen(private_file_name, "wt");
	if(p_private_file == NULL)
	{
		printf("DoGenerateRsaKey, open file %s err\n", private_file_name);
		exit(1);
	}

	char public_file_name[FILE_NAME_LEN];
	snprintf(public_file_name, FILE_NAME_LEN, "keys/%s_public.rsa",  key_pair_file);
	FILE *p_public_file;
	p_public_file = fopen(public_file_name, "wt");
	if(p_public_file == NULL)
	{
		printf("DoGenerateRsaKey, open file %s err\n", public_file_name);
		exit(1);
	}

	if(-1 == gmp_fprintf(p_public_file, "%Zd\n", e) || EOF == fputc('\n', p_public_file) 
		|| -1 == gmp_fprintf(p_private_file, "%Zd\n", d) || EOF == fputc('\n', p_private_file) 
		|| -1 == gmp_fprintf(p_public_file, "%Zd\n", n) || -1 == gmp_fprintf(p_private_file, "%Zd\n", n))
	{
		printf("DoGenerateRsaKey, write key to file err\n");
		exit(1);
	}

	fclose(p_private_file);
	fclose(p_public_file);
}

// static char *Crypt(const unsigned char *const source, unsigned char *dest, const BigInt *const key, const BigInt *const n)
// {
// 	BigInt s, d;

// 	StrToBigInt(source, &s);

// 	DoPowMod(&s, key, n, &d);

// 	return BigIntToStr(&d, dest);
// }

// char *RsaEncrypt(const unsigned char *const source, unsigned char *dest, const char *const key_pair_file)
// {
// 	/*  */
// 	char public_file_name[FILE_NAME_LEN];
// 	snprintf(public_file_name, FILE_NAME_LEN, "keys/%s_public.rsa",  key_pair_file);
// 	FILE *p_public_file;
// 	p_public_file = fopen(public_file_name, "rt");
// 	if(p_public_file == NULL)
// 	{
// 		printf("RsaEncrypt, open file %s err\n", public_file_name);
// 		exit(1);
// 	}

// 	/*  */
// 	char buffer[BIG_INT_BIT_LEN];
// 	BigInt e, n;
// 	char c;
// 	int mark = 0;
	
// 	while((fgets(buffer, BIG_INT_BIT_LEN, p_public_file)) != NULL)
// 	{
// 		if(mark == 0)
// 		{
// 			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
// 			buffer[real_size] = '\0';

// 			StrToBigInt(buffer, &e);
// 		}
// 		else
// 		{
// 			StrToBigInt(buffer, &n);
// 		}

// 		mark++;
// 	}
// 	if(!feof(p_public_file))
// 	{
// 		printf("RsaEncrypt, fgets err %s\n", public_file_name);
// 		exit(1);
// 	}
// 	fclose(p_public_file);

// 	return Crypt(source, dest, &e, &n);
// }

// char *RsaDecrypt(const unsigned char *const source, unsigned char *dest, const char *const key_pair_file)
// {
// 	/*  */
// 	char private_file_name[FILE_NAME_LEN];
// 	snprintf(private_file_name, FILE_NAME_LEN, "keys/%s_private.rsa",  key_pair_file);
// 	FILE *p_private_file;
// 	p_private_file = fopen(private_file_name, "rt");
// 	if(p_private_file == NULL)
// 	{
// 		printf("RsaEncrypt, open file %s err\n", private_file_name);
// 		exit(1);
// 	}

// 	/*  */
// 	char buffer[BIG_INT_BIT_LEN];
// 	BigInt d, n;
// 	char c;
// 	int mark = 0;
	
// 	while((fgets(buffer, BIG_INT_BIT_LEN, p_private_file)) != NULL)
// 	{
// 		if(mark == 0)
// 		{
// 			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
// 			buffer[real_size] = '\0';

// 			StrToBigInt(buffer, &d);
// 		}
// 		else
// 		{
// 			StrToBigInt(buffer, &n);
// 		}

// 		mark++;
// 	}
// 	if(!feof(p_private_file))
// 	{
// 		printf("RsaEncrypt, fgets err %s\n", private_file_name);
// 		exit(1);
// 	}
// 	fclose(p_private_file);

// 	return Crypt(source, dest, &d, &n);
// }

// char *RsaSign(const unsigned char *const source, unsigned char *dest, const char *const key_pair_file)
// {
// 	/*  */
// 	char private_file_name[FILE_NAME_LEN];
// 	snprintf(private_file_name, FILE_NAME_LEN, "keys/%s_private.rsa",  key_pair_file);
// 	FILE *p_private_file;
// 	p_private_file = fopen(private_file_name, "rt");
// 	if(p_private_file == NULL)
// 	{
// 		printf("RsaEncrypt, open file %s err\n", private_file_name);
// 		exit(1);
// 	}

// 	/*  */
// 	char buffer[BIG_INT_BIT_LEN];
// 	BigInt d, n;
// 	char c;
// 	int mark = 0;
	
// 	while((fgets(buffer, BIG_INT_BIT_LEN, p_private_file)) != NULL)
// 	{
// 		if(mark == 0)
// 		{
// 			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
// 			buffer[real_size] = '\0';

// 			StrToBigInt(buffer, &d);
// 		}
// 		else
// 		{
// 			StrToBigInt(buffer, &n);
// 		}

// 		mark++;
// 	}
// 	if(!feof(p_private_file))
// 	{
// 		printf("RsaEncrypt, fgets err %s\n", private_file_name);
// 		exit(1);
// 	}
// 	fclose(p_private_file);

// 	return Crypt(source, dest, &d, &n);
// }

// char *RsaVerifySign(const unsigned char *const source, unsigned char *dest, const char *const key_pair_file)
// {
// 	/*  */
// 	char public_file_name[FILE_NAME_LEN];
// 	snprintf(public_file_name, FILE_NAME_LEN, "keys/%s_public.rsa",  key_pair_file);
// 	FILE *p_public_file;
// 	p_public_file = fopen(public_file_name, "rt");
// 	if(p_public_file == NULL)
// 	{
// 		printf("RsaEncrypt, open file %s err\n", public_file_name);
// 		exit(1);
// 	}

// 	/*  */
// 	char buffer[BIG_INT_BIT_LEN];
// 	BigInt e, n;
// 	char c;
// 	int mark = 0;
	
// 	while((fgets(buffer, BIG_INT_BIT_LEN, p_public_file)) != NULL)
// 	{
// 		if(mark == 0)
// 		{
// 			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
// 			buffer[real_size] = '\0';

// 			StrToBigInt(buffer, &e);
// 		}
// 		else
// 		{
// 			StrToBigInt(buffer, &n);
// 		}

// 		mark++;
// 	}
// 	if(!feof(p_public_file))
// 	{
// 		printf("RsaEncrypt, fgets err %s\n", public_file_name);
// 		exit(1);
// 	}
// 	fclose(p_public_file);

// 	return Crypt(source, dest, &e, &n);
// }