/*
secp256k1
The curve I've chosen is secp256k1, from SECG (the "Standards for Efficient Cryptography Group", founded by Certicom). This same curve is also used by Bitcoin for digital signatures. 
y^2 = (x ^ 3 + a * x + b) mod p (p  is a prime)

*/

#include "../common.h"
#include <stdio.h>
#include <string.h>
#include "../hash/sha256.h"

static BigInt P;
static BigInt A;
static BigInt B;
static BigInt X_G;
static BigInt Y_G;
static BigInt N; /* the order of discrete ellipse curve, N * p = 0 (p is a random point which at descrete ellipse curve) */
static BigInt H; /* n * h = N, n * ( h * p) = 0, G = h * p (p is a random point which at descrete ellipse curve) */

// char PARA[7][BIG_INT_BIT_LEN] =
// {
// 	"fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f",
// 	"0",
// 	"7",
// 	"79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798",
// 	"483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8",
// 	"fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141",
// 	"1"
// };

char PARA[7][BIG_INT_BIT_LEN] =
{
	"61",
	"2",
	"3",
	"3",
	"6",
	"5",
	"1"
};

static BigInt ECC_ZERO;
static BigInt BIG_INT_ZERO;
static BigInt BIG_INT_ONE;
static BigInt BIG_INT_TWO;
static BigInt BIG_INT_THREE;
static char debug_tmp[BIG_INT_BIT_LEN];

void InitDomainParameters()
{
	char s_decimal_p[BIG_INT_BIT_LEN];
	char s_tmp_1[BIG_INT_BIT_LEN];
	char s_tmp_2[BIG_INT_BIT_LEN];
	ChangeStringRadix(PARA[0], 16, 10, s_decimal_p);
	StrToBigInt(s_decimal_p, &P);

	StrToBigInt(PARA[1], &A);

	StrToBigInt(PARA[2], &B);

	char s_decimal_x_g[BIG_INT_BIT_LEN];
	ChangeStringRadix(PARA[3], 16, 10, s_decimal_x_g);
	StrToBigInt(s_decimal_x_g, &X_G);

	char s_decimal_y_g[BIG_INT_BIT_LEN];
	ChangeStringRadix(PARA[4], 16, 10, s_decimal_y_g);
	StrToBigInt(s_decimal_y_g, &Y_G);

	char s_decimal_n[BIG_INT_BIT_LEN];
	ChangeStringRadix(PARA[5], 16, 10, s_decimal_n);
	StrToBigInt(s_decimal_n, &N);

	StrToBigInt(PARA[6], &H);

	StrToBigInt("-99", &ECC_ZERO);
	StrToBigInt("0", &BIG_INT_ZERO);
	StrToBigInt("1", &BIG_INT_ONE);
	StrToBigInt("2", &BIG_INT_TWO);
	StrToBigInt("3", &BIG_INT_THREE);
}

BigInt *GeneMulReverse(const BigInt *const a, const BigInt *const p, BigInt *mul_inverse)
{
	BigInt result, tmp, x, y;
	unsigned char a_big_p;

	if(DoCompare(a, p) > 0)
	{
		a_big_p = 1;
	}
	else
	{
		a_big_p = 0;
	}

	StrToBigInt("0", &tmp);
	
	/* check arg */
	if(DoCompare(a, &BIG_INT_ZERO) <= 0)
	{
		BigIntToStr(a, debug_tmp);
		printf("\nGeneMulReverse, a must bigger than zero %s\n", debug_tmp);
		exit(1);
	}

	if(DoCompare(p, &BIG_INT_ZERO) <= 0)
	{
		BigIntToStr(p, debug_tmp);
		printf("\nGeneMulReverse, p must bigger than zero %s\n", debug_tmp);
		exit(1);
	}

	while(1)
	{
			CopyBigInt(&tmp, &y);
			if(a_big_p)
			{
				DoExGcd(a, p, &x, &y, &result);
				if(DoCompare(&x, &BIG_INT_ZERO) > 0)
				{
					return CopyBigInt(&x, mul_inverse);
				}
			}
			else
			{
				DoExGcd(p, a, &x, &y, &result);
				if(DoCompare(&y, &BIG_INT_ZERO) > 0)
				{
					return CopyBigInt(&y, mul_inverse);
				}
			}

			if(DoCompare(&tmp, &BIG_INT_ZERO) == 0)
			{
				DoAdd(&tmp, &BIG_INT_ONE, &tmp);
			}
			else if(DoCompare(&tmp, &BIG_INT_ZERO) > 0)
			{
				DoSub(&BIG_INT_ZERO, &tmp, &tmp);
			}
			else
			{
				DoSub(&BIG_INT_ZERO, &tmp, &tmp);
				DoAdd(&tmp, &BIG_INT_ONE, &tmp);
			}
	}
}

static void ComputeXGAddYG(const BigInt *const _x_p, const BigInt *const _y_p, const BigInt *const _x_q, const BigInt *const _y_q, BigInt *result_x, BigInt *result_y)
{
	if(DoCompare(_x_p, _x_q) == 0 && DoCompare(_y_p, _y_q) == 0)
	{
		printf("ComputeXGAddYG, point p and q must be different\n");
		exit(1);
	}

	/* check zero */
	if(DoCompare(_x_p, &ECC_ZERO) == 0 || DoCompare(_y_p, &ECC_ZERO) == 0)
	{
		CopyBigInt(_x_q, result_x);
		CopyBigInt(_y_q, result_y);
		return;
	}

	if(DoCompare(_x_q, &ECC_ZERO) == 0 || DoCompare(_y_q, &ECC_ZERO) == 0)
	{
		CopyBigInt(_x_p, result_x);
		CopyBigInt(_y_p, result_y);
		return;
	}

	if(DoCompare(_x_p, _x_q) == 0)
	{
		StrToBigInt(&ECC_ZERO, result_x);
		StrToBigInt(&ECC_ZERO, result_y);
		return;
	}

	BigInt tmp_1, tmp_2, m, mod_inverse, x_p, y_p, x_q, y_q;

	CopyBigInt(_x_p, &x_p);
	CopyBigInt(_y_p, &y_p);
	CopyBigInt(_x_q, &x_q);
	CopyBigInt(_y_q, &y_q);

	/* compute slope m */
	DoSub(&y_p, &y_q, &tmp_1);
	DoSub(&x_p, &x_q, &tmp_2);
	if(DoCompare(&tmp_2, &BIG_INT_ZERO) < 0)
	{
		DoSub(&BIG_INT_ZERO, &tmp_2, &tmp_2);
	}

	GeneMulReverse(&tmp_2, &P, &mod_inverse);
	
	if(ECC_DEBUG)
	{
		BigIntToStr(&mod_inverse, debug_tmp);
		printf("\nComputeXGAddYG, mod_inverse %s\n", debug_tmp);
	}

	DoMul(&tmp_1, &mod_inverse, &tmp_1);
	DoMod(&tmp_1, &P, &m);
	if(DoCompare(&m, &BIG_INT_ZERO) < 0)
	{
		DoAdd(&m, &P, &m);
	}
	if(ECC_DEBUG)
	{
		BigIntToStr(&m, debug_tmp);
		printf("\nComputeXGAddYG, m %s\n", debug_tmp);
	}

	/* compute x */
	DoPow(&m, &BIG_INT_TWO, &tmp_1);
	DoSub(&tmp_1, &x_p, &tmp_1);
	DoSub(&tmp_1, &x_q, &tmp_1);
	DoMod(&tmp_1, &P, result_x);
	if(DoCompare(result_x, &BIG_INT_ZERO) < 0)
	{
		DoAdd(result_x, &P, result_x);
	}

	/* compute y */
	DoSub(result_x, &x_p, &tmp_1);
	DoMul(&tmp_1, &m, &tmp_1);
	DoAdd(&tmp_1, &y_p, &tmp_1);
	DoMod(&tmp_1, &P, result_y);
	if(DoCompare(result_y, &BIG_INT_ZERO) < 0)
	{
		DoAdd(result_y, &P, result_y);
	}
	DoSub(&P, result_y, result_y);


	if(ECC_DEBUG)
	{
		BigIntToStr(&x_p, debug_tmp);
		printf("\nComputeXGAddYG, x_p %s\n", debug_tmp);

		BigIntToStr(&y_p, debug_tmp);
		printf("ComputeXGAddYG, y_p %s\n", debug_tmp);

		BigIntToStr(&x_q, debug_tmp);
		printf("ComputeXGAddYG, x_q %s\n", debug_tmp);

		BigIntToStr(&y_q, debug_tmp);
		printf("ComputeXGAddYG, y_q %s\n", debug_tmp);

		BigIntToStr(result_x, debug_tmp);
		printf("ComputeXGAddYG, result_x %s\n", debug_tmp);

		BigIntToStr(result_y, debug_tmp);
		printf("ComputeXGAddYG, result_y %s\n", debug_tmp);
	}
}

/* double and add */
static void ComputeMP(const BigInt *const private_key, const BigInt *const origin_x, const BigInt *const origin_y, BigInt *p_x, BigInt *p_y)
{
	BigInt t_x, t_y, tmp_1, tmp_2, mod_inverse, m, r_x, r_y;
	int i, 
	compute_round = GetTrueValueLen(private_key), 
	init = 0;

	/* init origin point */
	CopyBigInt(origin_x, &t_x);
	CopyBigInt(origin_y, &t_y);

	/*  */
	if(ECC_DEBUG)
	{
		BigIntToStr(origin_x, debug_tmp);
		printf("ComputeMP, origin_x %s\n", debug_tmp);

		BigIntToStr(origin_y, debug_tmp);
		printf("ComputeMP, origin_y %s\n", debug_tmp);

		BigIntToStr(private_key, debug_tmp);
		printf("ComputeMP, private_key: %s, need round: %d\n", debug_tmp, compute_round);
	}

	for(i = 0; i < compute_round; i++)
	{
		if(private_key->bit[i] == 1)
		{
			if(init == 0)
			{
				CopyBigInt(&t_x, p_x);
				CopyBigInt(&t_y, p_y);

				init = 1;
			}
			else
			{
				if(DoCompare(&t_x, p_x) == 0 && DoCompare(&t_y, p_y) == 0)
				{
					ComputeMP(&BIG_INT_TWO, &t_x, &t_y, p_x, p_y);
				}
				else
				{
					ComputeXGAddYG(&t_x, &t_y, p_x, p_y, p_x, p_y);
				}
			}
		}

		/************************************************* compute slope m *************************************************/
		DoPow(&t_x, &BIG_INT_TWO, &tmp_1);
		DoMul(&BIG_INT_THREE, &tmp_1, &tmp_1);
		DoAdd(&tmp_1, &A, &tmp_1);
		/*  */
		DoMul(&t_y, &BIG_INT_TWO, &tmp_2);
		
		GeneMulReverse(&tmp_2, &P, &mod_inverse);
	
		if(ECC_DEBUG)
		{
			BigIntToStr(&mod_inverse, debug_tmp);
			printf("\nComputeMP, mod_inverse %s\n", debug_tmp);
		}

		/*  */
		DoMul(&tmp_1, &mod_inverse, &tmp_1);
		DoMod(&tmp_1, &P, &m);
		if(DoCompare(&m, &BIG_INT_ZERO) < 0)
		{
			DoAdd(&m, &P, &m);
		}
		if(ECC_DEBUG)
		{
			BigIntToStr(&m, debug_tmp);
			printf("\nComputeMP, m %s\n", debug_tmp);
		}

		/* compute x */
		DoPow(&m, &BIG_INT_TWO, &tmp_1);
		DoMul(&t_x, &BIG_INT_TWO, &tmp_2);
		DoSub(&tmp_1, &tmp_2, &tmp_1);
		DoMod(&tmp_1, &P, &r_x);
		if(DoCompare(&r_x, &BIG_INT_ZERO) < 0)
		{
			DoAdd(&r_x, &P, &r_x);
		}

		/* compute y */
		DoSub(&r_x, &t_x, &tmp_1);
		DoMul(&tmp_1, &m, &tmp_1);
		DoAdd(&tmp_1, &t_y, &tmp_1);
		DoMod(&tmp_1, &P, &r_y);
		if(DoCompare(&r_y, &BIG_INT_ZERO) < 0)
		{
			DoAdd(&r_y, &P, &r_y);
		}
		DoSub(&P, &r_y, &r_y);

		/*  */
		CopyBigInt(&r_x, &t_x);
		CopyBigInt(&r_y, &t_y);
		/*************************************************  *************************************************/

		if(ECC_DEBUG)
		{
			BigIntToStr(&r_x, debug_tmp);
			printf("\nComputeMP, 2^%d * P, r_x: %s\n", i + 1, debug_tmp);

			BigIntToStr(&r_y, debug_tmp);
			printf("ComputeMP, 2^%d * P, r_y: %s\n", i + 1, debug_tmp);
		}
	}

	if(ECC_DEBUG)
	{
		BigIntToStr(p_x, debug_tmp);
		printf("\nComputeMP, result_x %s\n", debug_tmp);

		BigIntToStr(p_y, debug_tmp);
		printf("ComputeMP, result_y %s\n", debug_tmp);
	}

	if(ECC_DEBUG)
	{
		printf("\n\n");
	}
}

void GenerateEccKey(const int byteLen, const char *const key_pair_file)
{
	printf("begin to generate ecc key ...\n");

	InitDomainParameters();

	BigInt p_x, p_y, private_key;

	char s_private_key[BIG_INT_BIT_LEN], s_public_key_x[BIG_INT_BIT_LEN], s_public_key_y[BIG_INT_BIT_LEN];
	char s_tmp_1[BIG_INT_BIT_LEN];
	char s_tmp_2[BIG_INT_BIT_LEN];

	/* init private key */
	DoGetPositiveRandBigInt(byteLen, &private_key);
	DoMod(&private_key, &N, &private_key);
	if(DoCompare(&private_key, &BIG_INT_ZERO) == 0 || DoCompare(&private_key, &BIG_INT_ONE) == 0)
	{
		CopyBigInt(&BIG_INT_TWO, &private_key);
	}
	BigIntToStr(&private_key, &s_private_key);
	/* compute public key */
	ComputeMP(&private_key, &X_G, &Y_G, &p_x, &p_y);

	/*  */
	BigIntToStr(&p_x, s_public_key_x);
	BigIntToStr(&p_y, s_public_key_y);

	/*********************************** generate key pair file ***********************************/
	char private_file_name[FILE_NAME_LEN];
	snprintf(private_file_name, FILE_NAME_LEN, "%s_private.ecc",  key_pair_file);
	FILE *p_private_file;
	p_private_file = fopen(private_file_name, "wt");
	if(p_private_file == NULL)
	{
		printf("GenerateEccKey, open file %s err\n", private_file_name);
		exit(1);
	}

	char public_file_name[FILE_NAME_LEN];
	snprintf(public_file_name, FILE_NAME_LEN, "%s_public.ecc",  key_pair_file);
	FILE *p_public_file;
	p_public_file = fopen(public_file_name, "wt");
	if(p_public_file == NULL)
	{
		printf("GenerateEccKey, open file %s err\n", public_file_name);
		exit(1);
	}

	if(EOF == fputs(s_public_key_x, p_public_file) || EOF == fputc('\n', p_public_file) 
		|| EOF == fputs(s_public_key_y, p_public_file) || EOF == fputc('\n', p_public_file) 
		|| EOF == fputs(s_private_key, p_private_file)  || EOF == fputc('\n', p_private_file)
		|| EOF == fputs(s_public_key_x, p_private_file)  || EOF == fputc('\n', p_private_file) 
		|| EOF == fputs(s_public_key_y, p_private_file)  || EOF == fputc('\n', p_private_file))
	{
		printf("GenerateEccKey, write e to file err\n");
		exit(1);
	}

	fclose(p_private_file);
	fclose(p_public_file);

	if(ECC_GENERATE_KEY_DEBUG)
	{
		printf("GenerateEccKey private_key: %s\npublic_x: %s\npublic_y: %s\n", s_private_key, s_public_key_x, s_public_key_y);
	}
}

void EccSign(const int byteLen, const unsigned char *const s_source, const char *const key_pair_file, char *s_r, char *s_s)
{
	printf("begin to sign ...\n");

	/*  */
	InitDomainParameters();

	/*********************************** init private key ************************************/
	char private_file_name[FILE_NAME_LEN];
	snprintf(private_file_name, FILE_NAME_LEN, "%s_private.ecc",  key_pair_file);
	FILE *p_private_file;
	p_private_file = fopen(private_file_name, "rt");
	if(p_private_file == NULL)
	{
		printf("EccSign, can not open file %s\n", private_file_name);
		exit(1);
	}

	/*  */
	char buffer[BIG_INT_BIT_LEN];
	BigInt private_key, public_p_x, public_p_y;
	char c;
	int mark = 0;
	
	while((fgets(buffer, BIG_INT_BIT_LEN, p_private_file)) != NULL)
	{
		if(mark == 0)
		{
			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
			buffer[real_size] = '\0';

			StrToBigInt(buffer, &private_key);
		}
		else if(mark == 1)
		{
			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
			buffer[real_size] = '\0';

			StrToBigInt(buffer, &public_p_x);
		}
		else
		{
			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
			buffer[real_size] = '\0';

			StrToBigInt(buffer, &public_p_y);
		}

		mark++;
	}
	if(!feof(p_private_file))
	{
		printf("EccSign, fgets err %s\n", private_file_name);
		exit(1);
	}
	fclose(p_private_file);

	/*********************************** sign ***********************************/
	BigInt source, p_x, p_y, tmp_1, tmp_2, k, r, s, truncated_hash;

	/*  */
	memset(source.bit, 0, BIG_INT_BIT_LEN);
	byteSequenceToBinBigInt(s_source, SHA256_BYTES, &source);

	/* compute truncated_hash */
	DoMod(&source, &N, &truncated_hash);
	if(ECC_SIGN_DEBUG)
	{
		BigIntToStr(&truncated_hash, debug_tmp);
		printf("EccSign, truncated_hash: %s\n\n", debug_tmp);
	}

	do
	{
		/* init r */
		do
		{
			DoGetPositiveRandBigInt(byteLen, &k);
			DoMod(&k, &N, &k);
			if(DoCompare(&k, &BIG_INT_ZERO) == 0)
			{
				continue;
			}

			if(ECC_SIGN_DEBUG)
			{
				BigIntToStr(&k, debug_tmp);
				printf("EccSign, k: %s\n\n", debug_tmp);
			}

			/* compute kG */
			ComputeMP(&k, &X_G, &Y_G, &p_x, &p_y);
			/* compute r */
			DoMod(&p_x, &N, &r);
		}
		while(DoCompare(&r, &BIG_INT_ZERO) == 0);

		if(ECC_SIGN_DEBUG)
		{
			BigIntToStr(&r, debug_tmp);
			printf("EccSign, r: %s\n\n", debug_tmp);
		}

		/* compute k ^ -1 */
		GeneMulReverse(&k, &N, &tmp_1);

		/* compute right */
		DoMul(&r, &private_key, &tmp_2);
		DoAdd(&tmp_2, &truncated_hash, &tmp_2);

		/*  */
		DoMul(&tmp_1, &tmp_2, &tmp_1);
		DoMod(&tmp_1, &N, &s);
	}
	while(DoCompare(&s, &BIG_INT_ZERO) == 0);

	if(ECC_SIGN_DEBUG)
	{
		BigIntToStr(&s, debug_tmp);
		printf("EccSign, s: %s\n\n", debug_tmp);
	}

	BigIntToStr(&r, s_r);
	BigIntToStr(&s, s_s);
}

int EccVerifySign(const unsigned char *const s_source, const char *const key_pair_file, const char *const s_r, const char *const s_s)
{
	InitDomainParameters();

	/*********************************** init public key ***********************************/
	char public_file_name[FILE_NAME_LEN];
	snprintf(public_file_name, FILE_NAME_LEN, "%s_public.ecc",  key_pair_file);
	FILE *p_public_file;
	p_public_file = fopen(public_file_name, "rt");
	if(p_public_file == NULL)
	{
		printf("EccVerifySign, can not open file %s\n", public_file_name);
		exit(1);
	}

	/*  */
	char buffer[BIG_INT_BIT_LEN];
	BigInt public_p_x, public_p_y;
	char c;
	int mark = 0;
	
	while((fgets(buffer, BIG_INT_BIT_LEN, p_public_file)) != NULL)
	{
		if(mark == 0)
		{
			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
			buffer[real_size] = '\0';

			StrToBigInt(buffer, &public_p_x);
		}
		else
		{
			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
			buffer[real_size] = '\0';

			StrToBigInt(buffer, &public_p_y);
		}

		mark++;
	}
	if(!feof(p_public_file))
	{
		printf("EccVerifySign, fgets err %s\n", public_file_name);
		exit(1);
	}
	fclose(p_public_file);

	/*********************************** verify ***********************************/
	BigInt source, tmp, truncated_hash, s, r, s_reverse, v1, v2, x_p, y_p, x_q, y_q, result_x, result_y;

	/*  */
	memset(source.bit, 0, BIG_INT_BIT_LEN);
	byteSequenceToBinBigInt(s_source, SHA256_BYTES, &source);
	
	StrToBigInt(s_s, &s);
	StrToBigInt(s_r, &r);

	/* compute truncated_hash */
	DoMod(&source, &N, &truncated_hash);
	if(ECC_VERIFY_SIGN_DEBUG)
	{
		BigIntToStr(&truncated_hash, debug_tmp);
		printf("EccVerifySign, truncated_hash: %s\n\n", debug_tmp);
	}

	/*  */
	GeneMulReverse(&s, &N, &s_reverse);

	/* compute v1 */
	DoMul(&s_reverse, &truncated_hash, &tmp);
	DoMod(&tmp, &N, &v1);

	/* compute v2 */
	DoMul(&s_reverse, &r, &tmp);
	DoMod(&tmp, &N, &v2);

	/* compute left */
	ComputeMP(&v1, &X_G, &Y_G, &x_p, &y_p);
	if(ECC_VERIFY_SIGN_DEBUG)
	{
		BigIntToStr(&x_p, debug_tmp);
		printf("EccVerifySign, x_p: %s\n\n", debug_tmp);
		BigIntToStr(&y_p, debug_tmp);
		printf("EccVerifySign, y_p: %s\n\n", debug_tmp);
	}

	/* compute right */
	ComputeMP(&v2, &public_p_x, &public_p_y, &x_q, &y_q);
	if(ECC_VERIFY_SIGN_DEBUG)
	{
		BigIntToStr(&x_q, debug_tmp);
		printf("EccVerifySign, x_q: %s\n\n", debug_tmp);
		BigIntToStr(&y_q, debug_tmp);
		printf("EccVerifySign, y_q: %s\n\n", debug_tmp);
	}

	/*  */
	if(DoCompare(&x_p, &x_q) == 0 && DoCompare(&y_p, &y_q) == 0)
	{
		ComputeMP(&BIG_INT_TWO, &x_p, &y_p, &result_x, &result_y);
	}
	else
	{
		ComputeXGAddYG(&x_p, &y_p, &x_q, &y_q, &result_x, &result_y);
	}
	
	/*  */
	DoMod(&result_x, &N, &tmp);

	if(DoCompare(&tmp, &r) == 0)
	{
		return 1;
	}
	return 0;
}