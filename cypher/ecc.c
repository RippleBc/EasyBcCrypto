/*

The curve I've chosen is secp256k1, from SECG (the "Standards for Efficient Cryptography Group", founded by Certicom). This same curve is also used by Bitcoin for digital signatures. 
y^2 = (x ^ 3 + a * x + b) mod p (p  is a prime)

*/

#include "../common.h"
#include <stdio.h>

static const BigInt P;
static const BigInt A;
static const BigInt B;
static const BigInt X_G;
static const BigInt Y_G;
static const BigInt N; /* the order of discrete ellipse curve, N * p = 0 (p is a random point which at descrete ellipse curve) */
static const BigInt H; /* n * h = N, n * ( h * p) = 0, G = h * p (p is a random point which at descrete ellipse curve) */

void init_domain_parameters()
{
	char s_decimal_p[BIG_INT_BIT_LEN];
	char s_tmp_1[BIG_INT_BIT_LEN];
	char s_tmp_2[BIG_INT_BIT_LEN];
	ChangeStringRadix("fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f", 16, 10, s_decimal_p);
	StrToBigInt(s_decimal_p, &P);

	StrToBigInt("0", &A);

	StrToBigInt("7", &B);

	char s_decimal_x_g[BIG_INT_BIT_LEN];
	ChangeStringRadix("79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798", 16, 10, s_decimal_x_g);
	StrToBigInt(s_decimal_x_g, &X_G);

	char s_decimal_y_g[BIG_INT_BIT_LEN];
	ChangeStringRadix("483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8", 16, 10, s_decimal_y_g);
	StrToBigInt(s_decimal_y_g, &Y_G);

	char s_decimal_n[BIG_INT_BIT_LEN];
	ChangeStringRadix("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141", 16, 10, s_decimal_n);
	StrToBigInt(s_decimal_n, &N);

	StrToBigInt("1", &H);
}

BigInt *GeneMulReverse(BigInt *a, BigInt *b, BigInt *x, BigInt *mul_reverse)
{
	BigInt result, zero, one, y, neg_y;

	StrToBigInt("0", &zero);
	StrToBigInt("1", &one);

	StrToBigInt("0", &y);
	
	while(1)
	{
			CopyBigInt(&y, mul_reverse);
			DoExGcd(a, b, x, mul_reverse, &result);
			if(DoCompare(mul_reverse, &zero) > 0)
			{
				break;
			}

			if(DoCompare(&y, &zero) == 0)
			{
				DoAdd(&y, &one, &y);
				continue;
			}
			
			DoSub(&zero, &y, &neg_y);
			CopyBigInt(&neg_y, mul_reverse);
			DoExGcd(a, b, x, mul_reverse, &result);
			if(DoCompare(mul_reverse, &zero) > 0)
			{
				break;
			}


			DoAdd(&y, &one, &y);
	}
	
	return mul_reverse;
}

void generate_ecc_key(char *key_pair_file)
{
	printf("begin to generate ecc key ...\n");

	BigInt tmp_1, tmp_2, m, two, three, p_x, p_y, r_x, r_y, mul_reverse, private_key, x;
	BigInt i;

	char s_private_key[BIG_INT_BIT_LEN], s_public_key_x[BIG_INT_BIT_LEN], s_public_key_y[BIG_INT_BIT_LEN], s_g_x[BIG_INT_BIT_LEN], s_g_y[BIG_INT_BIT_LEN];
	char s_tmp_1[BIG_INT_BIT_LEN];
	char s_tmp_2[BIG_INT_BIT_LEN];

	/*  */
	CopyBigInt(&X_G, &p_x);
	CopyBigInt(&Y_G, &p_y);
	StrToBigInt("0", &i);

	DoGetPositiveRand(&P, &private_key);
	BigIntToStr(&private_key, &s_private_key);

	if(ECC_DEBUG)
	{
		BigIntToStr(&p_x, s_tmp_1);
		BigIntToStr(&p_y, s_tmp_2);
		printf("origin p_x: %s\norigin p_y: %s\n\n", s_tmp_1, s_tmp_2);

		printf("private_key: %s\n\n", s_private_key);
	}
	
	while(DoCompare(&i, &private_key) < 0)
	{
		/* compute slope m */
		StrToBigInt("2", &two);
		StrToBigInt("3", &three);
		/*  */
		DoPow(&p_x, &two, &tmp_1);
		DoMul(&three, &tmp_1, &tmp_1);
		DoAdd(&tmp_1, &A, &tmp_1);
		/*  */
		DoMul(&p_y, &two, &tmp_2);

		if(DoCompare(&P, &tmp_2) > 0)
		{
			GeneMulReverse(&P, &tmp_2, &x, &mul_reverse);
		}
		else
		{
			GeneMulReverse(&tmp_2, &P, &mul_reverse, &x);
		}
		
		/*  */
		DoMul(&tmp_1, &tmp_2, &tmp_1);
		DoMod(&tmp_1, &P, &m);

		/* compute x */
		DoPow(&m, &two, &tmp_1);
		DoMul(&p_x, &two, &tmp_2);
		DoSub(&tmp_1, &tmp_2, &tmp_1);
		DoMod(&tmp_1, &P, &r_x);

		/* compute y */
		DoSub(&r_x, &p_x, &tmp_1);
		DoMul(&tmp_1, &m, &tmp_1);
		DoAdd(&tmp_1, &p_y, &tmp_1);
		DoMod(&tmp_1, &P, &r_y);

		/*  */
		CopyBigInt(&r_x, &p_x);
		CopyBigInt(&r_y, &p_y);

		if(ECC_DEBUG)
		{
			BigIntToStr(&r_x, s_tmp_1);
			BigIntToStr(&r_y, s_tmp_2);

			printf("r_x: %s\nr_y: %s\n\n", s_tmp_1, s_tmp_2);
		}
	}

	/*  */
	BigIntToStr(&X_G, s_g_x);
	BigIntToStr(&Y_G, s_g_y);

	/*  */
	BigIntToStr(&p_x, s_public_key_x);
	BigIntToStr(&p_x, s_public_key_y);

	/* generate key pair file */
	char private_file_name[FILE_NAME_LEN];
	snprintf(private_file_name, FILE_NAME_LEN, "%s_private.ecc",  key_pair_file);
	FILE *p_private_file;
	p_private_file = fopen(private_file_name, "wt");
	if(p_private_file == NULL)
	{
		printf("generate_ecc_key, open file %s err\n", private_file_name);
		exit(1);
	}

	char public_file_name[FILE_NAME_LEN];
	snprintf(public_file_name, FILE_NAME_LEN, "%s_public.ecc",  key_pair_file);
	FILE *p_public_file;
	p_public_file = fopen(public_file_name, "wt");
	if(p_public_file == NULL)
	{
		printf("generate_ecc_key, open file %s err\n", public_file_name);
		exit(1);
	}

	if(EOF == fputs(s_public_key_x, p_public_file) || EOF == fputc('\n', p_public_file) 
		|| EOF == fputs(s_public_key_y, p_public_file) || EOF == fputc('\n', p_public_file) 
		|| EOF == fputs(s_private_key, p_private_file)  || EOF == fputc('\n', p_private_file) 
		|| EOF == fputs(s_g_x, p_public_file) || EOF == fputc('\n', p_public_file) 
		|| EOF == fputs(s_g_y, p_public_file) || EOF == fputc('\n', p_public_file)
		|| EOF == fputs(s_g_x, p_private_file) || EOF == fputc('\n', p_private_file) 
		|| EOF == fputs(s_g_y, p_private_file) || EOF == fputc('\n', p_private_file))
	{
		printf("generate_ecc_key, write e to file err\n");
		exit(1);
	}

	fclose(p_private_file);
	fclose(p_public_file);
}