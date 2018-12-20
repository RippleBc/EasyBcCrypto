/*
secp256k1
The curve I've chosen is secp256k1, from SECG (the "Standards for Efficient Cryptography Group", founded by Certicom). This same curve is also used by Bitcoin for digital signatures. 
y^2 = (x ^ 3 + a * x + b) mod p (p  is a prime)

*/

#include "../common.h"
#include <stdio.h>
#include <string.h>
#include "../hash/sha256.h"

static mpz_t P;
static mpz_t A;
static mpz_t B;
static mpz_t X_G;
static mpz_t Y_G;
static mpz_t N; /* the order of discrete ellipse curve, N * p = 0 (p is a random point which at descrete ellipse curve) */
static mpz_t H; /* n * h = N, n * ( h * p) = 0, G = h * p (p is a random point which at descrete ellipse curve) */

static int TEST_ECC_DOMAIN_PARAMETER = 0;

char SECP_256_K1_PARA[7][MAX_STR_SIZE] =
{
	"fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f",
	"0",
	"7",
	"79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798",
	"483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8",
	"fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141",
	"1"
};

char TEST_PARA[7][MAX_STR_SIZE] =
{
	"61",
	"2",
	"3",
	"3",
	"6",
	"5",
	"1"
};

static mpz_t ECC_ZERO;

void InitDomainParameters()
{

	mpz_init_set_str(ECC_ZERO, "-99", 16);

	if(TEST_ECC_DOMAIN_PARAMETER)
	{
		mpz_init_set_str(P, TEST_PARA[0], 16);
		mpz_init_set_str(A, TEST_PARA[1], 16);
		mpz_init_set_str(B, TEST_PARA[2], 16);
		mpz_init_set_str(X_G, TEST_PARA[3], 16);
		mpz_init_set_str(Y_G, TEST_PARA[4], 16);
		mpz_init_set_str(N, TEST_PARA[5], 16);
		mpz_init_set_str(H, TEST_PARA[6], 16);
	}
	else
	{
		mpz_init_set_str(P, SECP_256_K1_PARA[0], 16);
		mpz_init_set_str(A, SECP_256_K1_PARA[1], 16);
		mpz_init_set_str(B, SECP_256_K1_PARA[2], 16);
		mpz_init_set_str(X_G, SECP_256_K1_PARA[3], 16);
		mpz_init_set_str(Y_G, SECP_256_K1_PARA[4], 16);
		mpz_init_set_str(N, SECP_256_K1_PARA[5], 16);
		mpz_init_set_str(H, SECP_256_K1_PARA[6], 16);
	}
}

void GeneMulReverse(const mpz_t const a, const mpz_t const p, mpz_t mul_inverse)
{
	/* check arg */
	// if(mpz_cmp(a, G_BIG_INT_ZERO) <= 0)
	// {
	// 	gmp_printf("\nGeneMulReverse, a must bigger than zero %Zd\n", a);
	// 	exit(1);
	// }

	// if(mpz_cmp(p, G_BIG_INT_ZERO) <= 0)
	// {
	// 	gmp_printf("\nGeneMulReverse, p must bigger than zero %Zd\n", p);
	// 	exit(1);
	// }

	mpz_t tmp;

	mpz_init(tmp);

	mpz_gcdext(G_BIG_INT_ONE, mul_inverse, tmp, a, p);
}

static void ComputeXGAddYG(const mpz_t const _x_p, const mpz_t const _y_p, const mpz_t const _x_q, const mpz_t const _y_q, mpz_t result_x, mpz_t result_y)
{
	if(mpz_cmp(_x_p, _x_q) == 0 && mpz_cmp(_y_p, _y_q) == 0)
	{
		printf("ComputeXGAddYG, point p and q must be different\n");
		exit(1);
	}

	/* check zero */
	if(mpz_cmp(_x_p, ECC_ZERO) == 0 || mpz_cmp(_y_p, ECC_ZERO) == 0)
	{
		mpz_set(result_x, _x_q);
		mpz_set(result_y, _y_q);
		return;
	}

	if(mpz_cmp(_x_q, ECC_ZERO) == 0 || mpz_cmp(_y_q, ECC_ZERO) == 0)
	{
		mpz_set(result_x, _x_p);
		mpz_set(result_y, _y_p);
		return;
	}

	if(mpz_cmp(_x_p, _x_q) == 0)
	{
		mpz_set_str(result_x, ECC_ZERO, 16);
		mpz_set_str(result_y, ECC_ZERO, 16);
		return;
	}


	/************************************ compute ************************************/
	mpz_t tmp_1, tmp_2, m, mod_inverse, x_p, y_p, x_q, y_q;

	mpz_init(tmp_1);
	mpz_init(tmp_2);
	mpz_init(m);
	mpz_init(mod_inverse);
	mpz_init(x_p);
	mpz_init(y_p);
	mpz_init(x_q);
	mpz_init(y_q);

	mpz_set(x_p, _x_p);
	mpz_set(y_p, _y_p);
	mpz_set(x_q, _x_q);
	mpz_set(y_q, _y_q);

	/* compute slope m */
	mpz_sub(tmp_1, y_p, y_q);
	mpz_sub(tmp_2, x_p, x_q);

	GeneMulReverse(tmp_2, P, mod_inverse);
	if(ECC_DEBUG)
	{
		gmp_printf("ComputeXGAddYG, mod_inverse: %Zd\n", mod_inverse);
	}

	mpz_mul(tmp_1, tmp_1, mod_inverse);
	mpz_mod(m, tmp_1, P);
	if(ECC_DEBUG)
	{
		gmp_printf("ComputeXGAddYG, m: %Zd\n", m);
	}

	/* compute x */
	mpz_mul(tmp_1, m, m);
	mpz_sub(tmp_1, tmp_1, x_p);
	mpz_sub(tmp_1, tmp_1, x_q);
	mpz_mod(result_x, tmp_1, P);

	/* compute y */
	mpz_sub(tmp_1, result_x, x_p);
	mpz_mul(tmp_1, tmp_1, m);
	mpz_add(tmp_1, y_p, tmp_1);
	mpz_mod(result_y, tmp_1, P);
	mpz_sub(result_y, P, result_y);

	if(ECC_DEBUG)
	{
		gmp_printf("ComputeXGAddYG, x_p %Zd\n", x_p);
		gmp_printf("ComputeXGAddYG, y_p %Zd\n", y_p);
		gmp_printf("ComputeXGAddYG, x_q %Zd\n", x_q);
		gmp_printf("ComputeXGAddYG, y_q %Zd\n", y_q);
		gmp_printf("ComputeXGAddYG, result_x %Zd\n", result_x);
		gmp_printf("ComputeXGAddYG, result_y %Zd\n", result_y);
	}
}

/* double and add */
static void ComputeMP(const mpz_t const private_key, const mpz_t const origin_x, const mpz_t const origin_y, mpz_t p_x, mpz_t p_y)
{
	/*  */
	if(ECC_DEBUG)
	{
		gmp_printf("ComputeMP, origin_x: %Zd\n", origin_x);
		gmp_printf("ComputeMP, origin_y: %Zd\n", origin_y);
		gmp_printf("ComputeMP, private_key: %Zd\n", private_key);
	}

	mpz_t t_x, t_y, tmp_1, tmp_2, mod_inverse, m, r_x, r_y;
	int i, init = 0;
	char binary_private_key[MAX_STR_SIZE];

	mpz_init(t_x);
	mpz_init(t_y);
	mpz_init(tmp_1);
	mpz_init(tmp_2);
	mpz_init(mod_inverse);
	mpz_init(m);
	mpz_init(r_x);
	mpz_init(r_y);

	/* convert private_key to binary format */
	mpz_get_str(binary_private_key, 2, private_key);

	if(ECC_DEBUG)
	{
		printf("ComputeMP, binary_private_key: %s\n", binary_private_key);
	}

	/*  */
	int compute_round = strnlen(binary_private_key, MAX_STR_SIZE);
	if(ECC_DEBUG)
	{
		gmp_printf("ComputeMP, compute round: %d\n", compute_round);
	}

	/* init origin point */
	mpz_set(t_x, origin_x);
	mpz_set(t_y, origin_y);

	for(i = compute_round - 1; i >= 0; i--)
	{
		if(binary_private_key[i] == '1')
		{
			if(init == 0)
			{
				mpz_set(p_x, t_x);
				mpz_set(p_y, t_y);

				init = 1;
			}
			else
			{
				if(mpz_cmp(t_x, p_x) == 0 && mpz_cmp(t_y, p_y) == 0)
				{
					ComputeMP(G_BIG_INT_TWO, t_x, t_y, p_x, p_y);
				}
				else
				{
					ComputeXGAddYG(t_x, t_y, p_x, p_y, p_x, p_y);
				}
			}
		}

		/************************************************* compute slope m *************************************************/
		mpz_mul(tmp_1, t_x, t_x);
		mpz_mul(tmp_1, G_BIG_INT_THREE, tmp_1);
		mpz_add(tmp_1, tmp_1, A);

		/*  */
		mpz_mul(tmp_2, t_y, G_BIG_INT_TWO);
		GeneMulReverse(tmp_2, P, mod_inverse);
		if(ECC_DEBUG)
		{
			gmp_printf("ComputeMP, mod_inverse: %Zd\n", mod_inverse);
		}

		/*  */
		mpz_mul(tmp_1, tmp_1, mod_inverse);
		mpz_mod(m, tmp_1, P);
		if(ECC_DEBUG)
		{
			gmp_printf("\nComputeMP, m: %Zd\n", m);
		}

		/* compute x */
		mpz_mul(tmp_1, m, m);
		mpz_mul(tmp_2, t_x, G_BIG_INT_TWO);
		mpz_sub(tmp_1, tmp_1, tmp_2);
		mpz_mod(r_x, tmp_1, P);
		if(mpz_cmp(r_x, G_BIG_INT_ZERO) < 0)
		{
			mpz_add(r_x, P, r_x);
		}

		/* compute y */
		mpz_sub(tmp_1, r_x, t_x);
		mpz_mul(tmp_1, m, tmp_1);
		mpz_add(tmp_1, t_y, tmp_1);
		mpz_mod(r_y, tmp_1, P);
		if(mpz_cmp(r_y, G_BIG_INT_ZERO) < 0)
		{
			/* compute -r = p + q */
			mpz_add(r_y, P, r_y);
		}
		/* compute r = -(-r) */
		mpz_sub(r_y, P, r_y);

		/*  */
		mpz_set(t_x, r_x);
		mpz_set(t_y, r_y);
		/*************************************************  *************************************************/

		if(ECC_DEBUG)
		{
			gmp_printf("ComputeMP, 2^%d * P, r_x: %Zd\n", i + 1, r_x);
			gmp_printf("ComputeMP, 2^%d * P, r_y: %Zd\n", i + 1, r_y);
		}
	}

	if(ECC_DEBUG)
	{
		gmp_printf("ComputeMP, result_x: %Zd\n", p_x);
		gmp_printf("ComputeMP, result_y: %Zd\n", p_y);
	}

	if(ECC_DEBUG)
	{
		printf("\n\n");
	}
}

void GenerateEccKey(const char *const key_pair_file)
{
	printf("begin to generate ecc key ...\n");

	InitDomainParameters();

	mpz_t p_x, p_y, private_key;

	mpz_init(p_x);
	mpz_init(p_y);
	mpz_init(private_key);

	/* init private key */
	do
	{
		DoGetPositiveRand(N, private_key);
	}
	while(mpz_cmp(private_key, G_BIG_INT_TWO) == 0);
	if(mpz_cmp(private_key, G_BIG_INT_ZERO) == 0)
	{
		mpz_set(private_key, G_BIG_INT_ONE);
	}

	/* compute public key */
	ComputeMP(private_key, X_G, Y_G, p_x, p_y);

	/*********************************** generate key pair file ***********************************/
	char private_file_name[FILE_NAME_LEN];
	snprintf(private_file_name, FILE_NAME_LEN, "keys/%s_private.ecc",  key_pair_file);
	FILE *p_private_file;
	p_private_file = fopen(private_file_name, "wt");
	if(p_private_file == NULL)
	{
		printf("GenerateEccKey, open file %s err\n", private_file_name);
		exit(1);
	}

	char public_file_name[FILE_NAME_LEN];
	snprintf(public_file_name, FILE_NAME_LEN, "keys/%s_public.ecc",  key_pair_file);
	FILE *p_public_file;
	p_public_file = fopen(public_file_name, "wt");
	if(p_public_file == NULL)
	{
		printf("GenerateEccKey, open file %s err\n", public_file_name);
		exit(1);
	}

	if(-1 == gmp_fprintf(p_public_file, "%Z02x", p_x)
		|| -1 == gmp_fprintf(p_public_file, "%Z02x", p_y)
		|| -1 == gmp_fprintf(p_private_file, "%Z02x", private_key)
		|| -1 == gmp_fprintf(p_private_file, "%Z02x", p_x) 
		|| -1 == gmp_fprintf(p_private_file, "%Z02x", p_y))
	{
		printf("GenerateEccKey, write e to file err\n");
		exit(1);
	}

	fclose(p_private_file);
	fclose(p_public_file);

	if(ECC_GENERATE_KEY_DEBUG)
	{
		printf("GenerateEccKey private_key: %Zd\npublic_x: %Zd\npublic_y: %Zd\n", private_key, p_x, p_y);
	}
}

// void EccSign(const int byteLen, const unsigned char *const s_source, const char *const key_pair_file, char *s_r, char *s_s)
// {
// 	printf("begin to sign ...\n");

// 	/*  */
// 	InitDomainParameters();

// 	/*********************************** init private key ************************************/
// 	char private_file_name[FILE_NAME_LEN];
// 	snprintf(private_file_name, FILE_NAME_LEN, "keys/%s_private.ecc",  key_pair_file);
// 	FILE *p_private_file;
// 	p_private_file = fopen(private_file_name, "rt");
// 	if(p_private_file == NULL)
// 	{
// 		printf("EccSign, can not open file %s\n", private_file_name);
// 		exit(1);
// 	}

// 	/*  */
// 	char buffer[MAX_STR_SIZE];
// 	BigInt private_key, public_p_x, public_p_y;
// 	char c;
// 	int mark = 0;
	
// 	while((fgets(buffer, MAX_STR_SIZE, p_private_file)) != NULL)
// 	{
// 		if(mark == 0)
// 		{
// 			int real_size = strnlen(buffer, MAX_STR_SIZE) - SLASH_N_SIZE;
// 			buffer[real_size] = '\0';

// 			StrToBigInt(buffer, &private_key);
// 		}
// 		else if(mark == 1)
// 		{
// 			int real_size = strnlen(buffer, MAX_STR_SIZE) - SLASH_N_SIZE;
// 			buffer[real_size] = '\0';

// 			StrToBigInt(buffer, &public_p_x);
// 		}
// 		else
// 		{
// 			int real_size = strnlen(buffer, MAX_STR_SIZE) - SLASH_N_SIZE;
// 			buffer[real_size] = '\0';

// 			StrToBigInt(buffer, &public_p_y);
// 		}

// 		mark++;
// 	}
// 	if(!feof(p_private_file))
// 	{
// 		printf("EccSign, fgets err %s\n", private_file_name);
// 		exit(1);
// 	}
// 	fclose(p_private_file);

// 	/*********************************** sign ***********************************/
// 	BigInt source, p_x, p_y, tmp_1, tmp_2, k, r, s, truncated_hash;
// 	memset(&s, 0, MAX_STR_SIZE);

// 	/*  */
// 	memset(source.bit, 0, MAX_STR_SIZE);
// 	byteSequenceToBinBigInt(s_source, SHA256_BYTES, &source);

// 	/* compute truncated_hash */
// 	DoMod(&source, &N, &truncated_hash);
// 	if(ECC_SIGN_DEBUG)
// 	{
// 		BigIntToStr(&private_key, debug_tmp);
// 		printf("EccSign, private_key: %s\n\n", debug_tmp);
// 		BigIntToStr(&truncated_hash, debug_tmp);
// 		printf("EccSign, truncated_hash: %s\n\n", debug_tmp);
// 	}

// 	do
// 	{
// 		/* init r */
// 		DoGetPositiveRandBigInt(byteLen, &k);
// 		DoMod(&k, &N, &k);
// 		if(mpz_cmp(&k, &G_BIG_INT_ZERO) == 0)
// 		{
// 			continue;
// 		}
// 		if(ECC_SIGN_DEBUG)
// 		{
// 			BigIntToStr(&k, debug_tmp);
// 			printf("EccSign, k: %s\n\n", debug_tmp);
// 		}

// 		/* compute kG */
// 		ComputeMP(&k, &X_G, &Y_G, &p_x, &p_y);
// 		/* compute r */
// 		DoMod(&p_x, &N, &r);

// 		if(mpz_cmp(&r, &G_BIG_INT_ZERO) == 0)
// 		{
// 			continue;
// 		}
// 		if(ECC_SIGN_DEBUG)
// 		{
// 			BigIntToStr(&r, debug_tmp);
// 			printf("EccSign, r: %s\n\n", debug_tmp);
// 		}

// 		/* compute k ^ -1 */
// 		GeneMulReverse(&k, &N, &tmp_1);

// 		/* compute right */
// 		DoMul(&r, &private_key, &tmp_2);
// 		DoAdd(&tmp_2, &truncated_hash, &tmp_2);

// 		/*  */
// 		DoMul(&tmp_1, &tmp_2, &tmp_1);
// 		DoMod(&tmp_1, &N, &s);
// 	}
// 	while(mpz_cmp(&s, &G_BIG_INT_ZERO) == 0);

// 	if(ECC_SIGN_DEBUG)
// 	{
// 		BigIntToStr(&s, debug_tmp);
// 		printf("EccSign, s: %s\n\n", debug_tmp);
// 	}

// 	BigIntToStr(&r, s_r);
// 	BigIntToStr(&s, s_s);
// }

// int EccVerifySign(const unsigned char *const s_source, const char *const key_pair_file, const char *const s_r, const char *const s_s)
// {
// 	InitDomainParameters();

// 	/*********************************** init public key ***********************************/
// 	char public_file_name[FILE_NAME_LEN];
// 	snprintf(public_file_name, FILE_NAME_LEN, "keys/%s_public.ecc",  key_pair_file);
// 	FILE *p_public_file;
// 	p_public_file = fopen(public_file_name, "rt");
// 	if(p_public_file == NULL)
// 	{
// 		printf("EccVerifySign, can not open file %s\n", public_file_name);
// 		exit(1);
// 	}

// 	/*  */
// 	char buffer[MAX_STR_SIZE];
// 	BigInt public_p_x, public_p_y;
// 	char c;
// 	int mark = 0;
	
// 	while((fgets(buffer, MAX_STR_SIZE, p_public_file)) != NULL)
// 	{
// 		if(mark == 0)
// 		{
// 			int real_size = strnlen(buffer, MAX_STR_SIZE) - SLASH_N_SIZE;
// 			buffer[real_size] = '\0';

// 			StrToBigInt(buffer, &public_p_x);
// 		}
// 		else
// 		{
// 			int real_size = strnlen(buffer, MAX_STR_SIZE) - SLASH_N_SIZE;
// 			buffer[real_size] = '\0';

// 			StrToBigInt(buffer, &public_p_y);
// 		}

// 		mark++;
// 	}
// 	if(!feof(p_public_file))
// 	{
// 		printf("EccVerifySign, fgets err %s\n", public_file_name);
// 		exit(1);
// 	}
// 	fclose(p_public_file);

// 	/*********************************** verify ***********************************/
// 	BigInt source, tmp, truncated_hash, s, r, s_reverse, v1, v2, x_p, y_p, x_q, y_q, result_x, result_y;

// 	/*  */
// 	memset(source.bit, 0, MAX_STR_SIZE);
// 	byteSequenceToBinBigInt(s_source, SHA256_BYTES, &source);
	
// 	StrToBigInt(s_s, &s);
// 	StrToBigInt(s_r, &r);

// 	/* compute truncated_hash */
// 	DoMod(&source, &N, &truncated_hash);
// 	if(ECC_VERIFY_SIGN_DEBUG)
// 	{
// 		BigIntToStr(&truncated_hash, debug_tmp);
// 		printf("EccVerifySign, truncated_hash: %s\n\n", debug_tmp);
// 	}

// 	/*  */
// 	GeneMulReverse(&s, &N, &s_reverse);

// 	/* compute v1 */
// 	DoMul(&s_reverse, &truncated_hash, &tmp);
// 	DoMod(&tmp, &N, &v1);
// 	if(ECC_VERIFY_SIGN_DEBUG)
// 	{
// 		BigIntToStr(&v1, debug_tmp);
// 		printf("EccVerifySign, v1: %s\n\n", debug_tmp);
// 	}

// 	/* compute v2 */
// 	DoMul(&s_reverse, &r, &tmp);
// 	DoMod(&tmp, &N, &v2);
// 	if(ECC_VERIFY_SIGN_DEBUG)
// 	{
// 		BigIntToStr(&v2, debug_tmp);
// 		printf("EccVerifySign, v2: %s\n\n", debug_tmp);
// 	}

// 	/* compute left */
// 	ComputeMP(&v1, &X_G, &Y_G, &x_p, &y_p);
// 	if(ECC_VERIFY_SIGN_DEBUG)
// 	{
// 		BigIntToStr(&x_p, debug_tmp);
// 		printf("EccVerifySign, x_p: %s\n\n", debug_tmp);
// 		BigIntToStr(&y_p, debug_tmp);
// 		printf("EccVerifySign, y_p: %s\n\n", debug_tmp);
// 	}

// 	/* compute right */
// 	ComputeMP(&v2, &public_p_x, &public_p_y, &x_q, &y_q);
// 	if(ECC_VERIFY_SIGN_DEBUG)
// 	{
// 		BigIntToStr(&x_q, debug_tmp);
// 		printf("EccVerifySign, x_q: %s\n\n", debug_tmp);
// 		BigIntToStr(&y_q, debug_tmp);
// 		printf("EccVerifySign, y_q: %s\n\n", debug_tmp);
// 	}

// 	/*  */
// 	if(mpz_cmp(&x_p, &x_q) == 0 && mpz_cmp(&y_p, &y_q) == 0)
// 	{
// 		ComputeMP(&G_BIG_INT_TWO, &x_p, &y_p, &result_x, &result_y);
// 	}
// 	else
// 	{
// 		ComputeXGAddYG(&x_p, &y_p, &x_q, &y_q, &result_x, &result_y);
// 	}
	
// 	/*  */
// 	DoMod(&result_x, &N, &tmp);

// 	if(mpz_cmp(&tmp, &r) == 0)
// 	{
// 		return 1;
// 	}
// 	return 0;
// }