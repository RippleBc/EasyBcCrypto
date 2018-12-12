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
}

BigInt *GeneMulReverse(const BigInt *const a, const BigInt *const b, BigInt *x, BigInt *y)
{
	BigInt result, zero, one, tmp, neg_tmp;
	char debug_tmp[BIG_INT_BIT_LEN];

	StrToBigInt("0", &zero);
	StrToBigInt("1", &one);

	StrToBigInt("0", &tmp);
	
	if(DoCompare(a, &zero) <= 0)
	{
		BigIntToStr(a, debug_tmp);
		printf("\nGeneMulReverse, a must bigger than zero %s\n", debug_tmp);
		exit(1);
	}

	if(DoCompare(b, &zero) <= 0)
	{
		BigIntToStr(b, debug_tmp);
		printf("\nGeneMulReverse, b must bigger than zero %s\n", debug_tmp);
		exit(1);
	}

	while(1)
	{
			CopyBigInt(&tmp, y);
			DoExGcd(a, b, x, y, &result);
			if(DoCompare(y, &zero) > 0)
			{
				break;
			}

			if(DoCompare(&tmp, &zero) == 0)
			{
				DoAdd(&tmp, &one, &tmp);
				continue;
			}
			
			DoSub(&zero, &tmp, &neg_tmp);
			CopyBigInt(&neg_tmp, y);

			DoExGcd(a, b, x, y, &result);
			if(DoCompare(y, &zero) > 0)
			{
				break;
			}


			DoAdd(&tmp, &one, &tmp);
	}
	
	return y;
}

static void ComputeXGAddYG(const BigInt *const _x_p, const BigInt *const _y_p, const BigInt *const _x_q, const BigInt *const _y_q, BigInt *result_x, BigInt *result_y)
{
	BigInt tmp, tmp_1, tmp_2, m, mod_inverse, x_p, y_p, x_q, y_q;
	BigInt zero, two;

	CopyBigInt(_x_p, &x_p);
	CopyBigInt(_y_p, &y_p);
	CopyBigInt(_x_q, &x_q);
	CopyBigInt(_y_q, &y_q);

	char s_tmp_1[BIG_INT_BIT_LEN];
	char s_tmp_2[BIG_INT_BIT_LEN];

	StrToBigInt("0", &zero);
	StrToBigInt("2", &two);

	/* compute slope m */
	DoSub(&y_p, &y_q, &tmp_1);
	DoSub(&x_p, &x_q, &tmp_2);
	if(DoCompare(&tmp_2, &zero) < 0)
	{
		DoSub(&zero, &tmp_2, &tmp_2);
	}

	if(DoCompare(&P, &tmp_2) > 0)
	{
		GeneMulReverse(&P, &tmp_2, &tmp, &mod_inverse);
	}
	else
	{
		GeneMulReverse(&tmp_2, &P, &mod_inverse, &tmp);
	}
	
	DoMul(&tmp_1, &mod_inverse, &tmp_1);
	DoMod(&tmp_1, &P, &m);
	if(DoCompare(&m, &zero) < 0)
	{
		DoAdd(&m, &P, &m);
	}

	/* compute x */
	DoPow(&m, &two, &tmp_1);
	DoSub(&tmp_1, &x_p, &tmp_1);
	DoSub(&tmp_1, &x_q, &tmp_1);
	DoMod(&tmp_1, &P, &result_x);
	if(DoCompare(&result_x, &zero) < 0)
	{
		DoAdd(&result_x, &P, &result_x);
	}

	/* compute y */
	DoSub(&result_x, &x_p, &tmp_1);
	DoMul(&tmp_1, &m, &tmp_1);
	DoAdd(&tmp_1, &y_p, &tmp_1);
	DoMod(&tmp_1, &P, &result_y);
	if(DoCompare(&result_y, &zero) < 0)
	{
		DoAdd(&result_y, &P, &result_y);
	}
}

/* double and add */
static void ComputeMP(const BigInt *const private_key, BigInt *p_x, BigInt *p_y, const BigInt *const origin_x, const BigInt *const origin_y)
{
	BigInt t_x, t_y, tmp, tmp_1, tmp_2, mod_inverse, m, r_x, r_y;
	BigInt zero, one, two, three;
	int i, true_len, init = 0;

	true_len = GetTrueValueLen(private_key);

	char s_tmp_1[BIG_INT_BIT_LEN];
	char s_tmp_2[BIG_INT_BIT_LEN];
	char debug_tmp[BIG_INT_BIT_LEN];

	StrToBigInt("0", &zero);
	StrToBigInt("1", &one);
	StrToBigInt("2", &two);
	StrToBigInt("3", &three);

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
		printf("ComputeMP, M %s, need round %d, begin ", debug_tmp, true_len);
	}

	for(i = 0; i < true_len; i++)
	{
		if(private_key->bit[i] == 1)
		{
			if(init == 0)
			{
				CopyBigInt(&t_x, p_x);
				CopyBigInt(&t_y, p_y);

				init = 1;

				if(ECC_DEBUG)
				{
					BigIntToStr(&t_x, debug_tmp);
					printf("\nComputeMP, init, t_x %s\n", debug_tmp);

					BigIntToStr(&t_y, debug_tmp);
					printf("ComputeMP, init, t_y %s\n", debug_tmp);
				}
			}
			else
			{
				if(ECC_DEBUG)
				{
					BigIntToStr(&t_x, debug_tmp);
					printf("ComputeMP, ComputeXGAddYG before, t_x %s\n", debug_tmp);

					BigIntToStr(&t_y, debug_tmp);
					printf("ComputeMP, ComputeXGAddYG before, t_y %s\n", debug_tmp);

					BigIntToStr(p_x, debug_tmp);
					printf("ComputeMP, ComputeXGAddYG before, p_x %s\n", debug_tmp);

					BigIntToStr(p_y, debug_tmp);
					printf("ComputeMP, ComputeXGAddYG before, p_y %s\n", debug_tmp);
				}

				ComputeXGAddYG(&t_x, &t_y, p_x, p_y, p_x, p_y);

				if(ECC_DEBUG)
				{
					BigIntToStr(p_x, debug_tmp);
					printf("ComputeMP, ComputeXGAddYG after, p_x %s\n", debug_tmp);

					BigIntToStr(p_y, debug_tmp);
					printf("ComputeMP, ComputeXGAddYG after, p_y %s\n", debug_tmp);
				}
				
			}
		}

		/************************************************* compute slope m *************************************************/
		DoPow(&t_x, &two, &tmp_1);
		DoMul(&three, &tmp_1, &tmp_1);
		DoAdd(&tmp_1, &A, &tmp_1);
		/*  */
		DoMul(&t_y, &two, &tmp_2);
		
		if(DoCompare(&P, &tmp_2) > 0)
		{
			GeneMulReverse(&P, &tmp_2, &tmp, &mod_inverse);
		}
		else
		{
			GeneMulReverse(&tmp_2, &P, &mod_inverse, &tmp);
		}

		/*  */
		DoMul(&tmp_1, &mod_inverse, &tmp_1);
		DoMod(&tmp_1, &P, &m);
		if(DoCompare(&m, &zero) < 0)
		{
			DoAdd(&m, &P, &m);
		}

		/* compute x */
		DoPow(&m, &two, &tmp_1);
		DoMul(&t_x, &two, &tmp_2);
		DoSub(&tmp_1, &tmp_2, &tmp_1);
		DoMod(&tmp_1, &P, &r_x);
		if(DoCompare(&r_x, &zero) < 0)
		{
			DoAdd(&r_x, &P, &r_x);
		}

		/* compute y */
		DoSub(&r_x, &t_x, &tmp_1);
		DoMul(&tmp_1, &m, &tmp_1);
		DoAdd(&tmp_1, &t_y, &tmp_1);
		DoMod(&tmp_1, &P, &r_y);
		if(DoCompare(&r_y, &zero) < 0)
		{
			DoAdd(&r_y, &P, &r_y);
		}

		/*  */
		CopyBigInt(&r_x, &t_x);
		CopyBigInt(&r_y, &t_y);
		/*************************************************  *************************************************/

		if(ECC_DEBUG)
		{
			printf(".");
			fflush(stdout);
		}
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

	char s_private_key[BIG_INT_BIT_LEN], s_public_key_x[BIG_INT_BIT_LEN], s_public_key_y[BIG_INT_BIT_LEN], s_g_x[BIG_INT_BIT_LEN], s_g_y[BIG_INT_BIT_LEN];
	char s_tmp_1[BIG_INT_BIT_LEN];
	char s_tmp_2[BIG_INT_BIT_LEN];

	/*  */
	BigIntToStr(&X_G, s_g_x);
	BigIntToStr(&Y_G, s_g_y);

	/* init private key */
	DoGetPositiveRandBigInt(byteLen, &private_key);
	DoMod(&private_key, &N, &private_key);
	BigIntToStr(&private_key, &s_private_key);
	
	ComputeMP(&private_key, &p_x, &p_y, &X_G, &Y_G);

	/*  */
	BigIntToStr(&p_x, s_public_key_x);
	BigIntToStr(&p_y, s_public_key_y);

	/* generate key pair file */
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
		|| EOF == fputs(s_public_key_y, p_private_file)  || EOF == fputc('\n', p_private_file)  
		|| EOF == fputs(s_g_x, p_public_file) || EOF == fputc('\n', p_public_file) 
		|| EOF == fputs(s_g_y, p_public_file) || EOF == fputc('\n', p_public_file)
		|| EOF == fputs(s_g_x, p_private_file) || EOF == fputc('\n', p_private_file) 
		|| EOF == fputs(s_g_y, p_private_file) || EOF == fputc('\n', p_private_file))
	{
		printf("GenerateEccKey, write e to file err\n");
		exit(1);
	}

	fclose(p_private_file);
	fclose(p_public_file);
}

void EccSign(const int byteLen, const char *const s_source, const char *const key_pair_file, char *s_r, char *s_s)
{
	printf("begin to sign ...\n");

	InitDomainParameters();

	/*  */
	char private_file_name[FILE_NAME_LEN];
	snprintf(private_file_name, FILE_NAME_LEN, "%s_private.ecc",  key_pair_file);
	FILE *p_private_file;
	p_private_file = fopen(private_file_name, "rt");
	if(p_private_file == NULL)
	{
		printf("EccSign, open file %s err\n", private_file_name);
		exit(1);
	}

	/*  */
	char buffer[BIG_INT_BIT_LEN];
	BigInt private_key, public_p_x, public_p_y, origin_p_x, origin_p_y;
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
		else if(mark == 2)
		{
			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
			buffer[real_size] = '\0';

			StrToBigInt(buffer, &public_p_y);
		}
		else if(mark == 3)
		{
			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
			buffer[real_size] = '\0';

			StrToBigInt(buffer, &origin_p_x);
		}
		else
		{
			StrToBigInt(buffer, &origin_p_y);
		}

		mark++;
	}
	if(!feof(p_private_file))
	{
		printf("EccSign, fgets err %s\n", private_file_name);
		exit(1);
	}
	fclose(p_private_file);

	/* sign */
	BigInt source, tmp, p_x, p_y, zero, left, right, k, r, s, truncated_hash;
	char debug_tmp[BIG_INT_BIT_LEN];

	byteSequenceToBinBigInt(s_source, SHA256_BYTES, &source);
	StrToBigInt("0", &zero);

	/* compute truncated_hash */
	DoMod(&source, &N, &truncated_hash);

	do
	{
		do
		{
			/* init k */
			DoGetPositiveRandBigInt(byteLen, &k);
			DoMod(&k, &P, &k);

			if(ECC_DEBUG)
			{
				BigIntToStr(&k, debug_tmp);
				printf("EccSign, k: %s\n\n", debug_tmp);
			}

			/* compute kG */
			ComputeMP(&k, &p_x, &p_y, &X_G, &Y_G);
			/* compute r */
			DoMod(&p_x, &N, &r);
		}
		while(DoCompare(&r, &zero) == 0);

		if(ECC_DEBUG)
		{
			BigIntToStr(&r, debug_tmp);
			printf("EccSign, r: %s\n\n", debug_tmp);
		}

		/* compute k ^ -1 */
		if(DoCompare(&k, &N) > 0)
		{
			GeneMulReverse(&k, &N, &left, &tmp);
		}
		else
		{
			GeneMulReverse(&N, &k, &tmp, &left);
		}

		/* compute right */
		DoMul(&r, &private_key, &tmp);
		DoAdd(&tmp, &truncated_hash, &right);

		/*  */
		DoMul(&left, &right, &tmp);
		DoMod(&tmp, &N, &s);
	}
	while(DoCompare(&s, &zero) == 0);

	if(ECC_DEBUG)
	{
		BigIntToStr(&s, debug_tmp);
		printf("EccSign, s: %s\n\n", debug_tmp);
	}

	BigIntToStr(&r, s_r);
	BigIntToStr(&s, s_s);
}

int EccVerifySign(const char *const s_source, const char *const key_pair_file, const char *const s_r, const char *const s_s)
{
	InitDomainParameters();

	/*  */
	char public_file_name[FILE_NAME_LEN];
	snprintf(public_file_name, FILE_NAME_LEN, "%s_public.ecc",  key_pair_file);
	FILE *p_public_file;
	p_public_file = fopen(public_file_name, "rt");
	if(p_public_file == NULL)
	{
		printf("EccVerifySign, open file %s err\n", public_file_name);
		exit(1);
	}

	/*  */
	char buffer[BIG_INT_BIT_LEN];
	BigInt public_p_x, public_p_y, origin_p_x, origin_p_y;
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
		else if(mark == 1)
		{
			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
			buffer[real_size] = '\0';

			StrToBigInt(buffer, &public_p_y);
		}

		else if(mark == 2)
		{
			int real_size = strnlen(buffer, BIG_INT_BIT_LEN) - SLASH_N_SIZE;
			buffer[real_size] = '\0';

			StrToBigInt(buffer, &origin_p_x);
		}

		else if(mark == 3)
		{
			StrToBigInt(buffer, &origin_p_y);
		}

		mark++;
	}
	if(!feof(p_public_file))
	{
		printf("EccVerifySign, fgets err %s\n", public_file_name);
		exit(1);
	}
	fclose(p_public_file);

	/* verify */
	BigInt source, tmp, truncated_hash, s, r, s_reverse, v1, v2, x_p, y_p, x_q, y_q, result_x, result_y;
	char debug_tmp[BIG_INT_BIT_LEN];

	byteSequenceToBinBigInt(s_source, SHA256_BYTES, &source);
	StrToBigInt(s_s, &s);
	StrToBigInt(s_r, &r);

	/* compute truncated_hash */
	DoMod(&source, &N, &truncated_hash);	

	/*  */
	if(DoCompare(&s, &N) > 0)
	{
		GeneMulReverse(&s, &N, &s_reverse, &tmp);
	}
	else
	{
		GeneMulReverse(&N, &s, &tmp, &s_reverse);
	}

	/* compute v1 */
	DoMul(&s_reverse, &truncated_hash, &tmp);
	DoMod(&tmp, &N, &v1);

	/* compute v2 */
	DoMul(&s_reverse, &r, &tmp);
	DoMod(&tmp, &N, &v2);

	/* compute left */
	ComputeMP(&v1, &x_p, &y_p, &X_G, &Y_G);
	if(ECC_DEBUG)
	{
		BigIntToStr(&x_p, debug_tmp);
		printf("EccVerifySign, x_p: %s\n\n", debug_tmp);
		BigIntToStr(&y_p, debug_tmp);
		printf("EccVerifySign, y_p: %s\n\n", debug_tmp);
	}

	/* compute right */
	ComputeMP(&v2, &x_q, &y_q, &public_p_x, &public_p_y);
	if(ECC_DEBUG)
	{
		BigIntToStr(&x_q, debug_tmp);
		printf("EccVerifySign, x_q: %s\n\n", debug_tmp);
		BigIntToStr(&y_q, debug_tmp);
		printf("EccVerifySign, y_q: %s\n\n", debug_tmp);
	}

	/*  */
	ComputeXGAddYG(&x_p, &y_p, &x_q, &y_q, &result_x, &result_y);

	/*  */
	DoMod(&result_x, &N, &tmp);

	if(DoCompare(&tmp, &r) == 0)
	{
		return 1;
	}
	return 0;
}