#include "../common.h"
#include <stdio.h>

#define COMMON_PRIME_SIZE 8
static char common_prime[COMMON_PRIME_SIZE][10] = {
    "2", "3", "5", "7", "11", "13", "17", "19"
};

BigInt* DoGetPositiveOddRandBigInt(int byteLen, BigInt* result)
{
    int i;
    memset(result->bit, 0, BIG_INT_BIT_LEN);
    
    /*  */
    unsigned char random_byte_sequence[byteLen];

    /*  */
    get_specified_size_random(random_byte_sequence, byteLen);

    if(RANDOM_DEBUG)
    {
        printf("hex odd random result begin\n");
        for(i = 0; i < byteLen; i++)
        {
            printf("%x ", random_byte_sequence[i]);
        }
        printf("\nhex odd random result end\n\n");
    }

    /*  */
    byteSequenceToBinBigInt(random_byte_sequence, byteLen, result);

    /* convert to odd positive integer */
    result->bit[0] = 1;
    result->bit[SIGN_BIT] = 0;

    /*  */
    return result;
}

BigInt* DoGetPositiveRand(BigInt *n, BigInt *result)
{
    /*  */
    int i;
    BigInt tmp;

    /*  */
    memset(result->bit, 0, BIG_INT_BIT_LEN);

    /*  */
    int random_byte_sequence_size = floor(BIG_INT_BIT_LEN / BYTE_SIZE);
    
    
    /*  */
    unsigned char random_byte_sequence[random_byte_sequence_size];

    /*  */
    get_specified_size_random(random_byte_sequence, random_byte_sequence_size);

    if(RANDOM_DEBUG)
    {
        printf("hex random result begin\n");
        for(i = 0; i < random_byte_sequence_size; i ++)
        {
            printf("%x ", random_byte_sequence[i]);
        }
        printf("\nhex random result end\n\n");
    }

    /*  */
    byteSequenceToBinBigInt(random_byte_sequence, random_byte_sequence_size, &tmp);

    /* convert to positive integer */
    tmp.bit[SIGN_BIT] = 0;

    /*  */
    return DoMod(&tmp, n, result);
}

/*

费马小定理：对于素数p和任意整数a，有a ^ (p-1) = 1 (mod p)（同余）。反过来，满足a ^ (p-1) = 1 (mod p)，p也几乎一定是素数。

伪素数：如果n是一个正整数，如果存在和p互素的正整数a满足a ^ (p-1) = 1 (mod p)，我们说n是基于a的伪素数。如果一个数是伪素数，那么它几乎肯定是素数。

Miller-Rabin测试：不断选取不超过p-1的基a(s次)，计算是否每次都有a ^ (p-1) = 1 (mod p)，若每次都成立则p是素数，否则为合数。如果p是素数，x是小于p的正整数，且 x^2 = 1(mod p)，那么要么x=1，要么x=p-1;

模运算与基本四则运算有些相似，但是除法例外。其规则如下：
(a + b) % p = (a % p + b % p) % p
(a - b) % p = (a % p - b % p) % p
(a * b) % p = (a % p * b % p) % p
(a^b) % p = ((a % p)^b) % p

推论：
若a≡b (% p)，则对于任意的c，都有(a + c) ≡ (b + c) (%p)；
若a≡b (% p)，则对于任意的c，都有(a * c) ≡ (b * c) (%p)；
若a≡b (% p)，c≡d (% p)，则 (a + c) ≡ (b + d) (%p)，(a - c) ≡ (b - d) (%p)，
(a * c) ≡ (b * d) (%p)，(a / c) ≡ (b / d) (%p)； 

*/
int DoMillerRabin(BigInt *p, int times)
{
    int i, j, z, miller_rabin_max_test_time;
    BigInt result, base;
    BigInt one, two, pMinusOne, tmp_pMinusOne, remainder;
    char s_p[BIG_INT_BIT_LEN];

    /*  */
    BigIntToStr(p, s_p);
    for(i = 0; i < COMMON_PRIME_SIZE; i++)
    {
        if(strcmp(s_p, common_prime[i] == 0))
        {
            return 1;
        }
    }   

    char tmp_decimal_big_int[BIG_INT_BIT_LEN];

    StrToBigInt("1", &one);
    StrToBigInt("2", &two);
    DoSub(p, &one, &pMinusOne);

    /* Get Max Miller-Rabin test times */
    miller_rabin_max_test_time = GetMaxRightShiftLen(&pMinusOne);

    /*  */
    if(MILLER_RABIN_DEBUG)
    {
        printf("\np: ");
        BigIntToStr(p, tmp_decimal_big_int);
        for(j = 0; j < strlen(tmp_decimal_big_int); j++)
        {
            printf("%c", tmp_decimal_big_int[j]);
        }
        printf("\n");
    }
    

    /* do {times} test, {times} is bigger, more accuracy */
    for(i = 0; i < times; i++)
    {
        do
        {
            DoGetPositiveRand(p, &base);
        }
        while(DoCompare(&base, &two) < 0);
        
        if(MILLER_RABIN_DEBUG)
        {
            printf("base: ");
            BigIntToStr(&base, tmp_decimal_big_int);
            for(j = 0; j < strlen(tmp_decimal_big_int); j++)
            {
                printf("%c", tmp_decimal_big_int[j]);
            }
            printf("\n");
        }

        /* Fermat test */
        DoPowMod(&base, &pMinusOne, p, &result); /* result = base ^ (p - 1) % p */

        if(DoCompare(&result, &one) != 0)
        {
            /* not prime */
            return 0;
        }
       
        /* Miller-Rabin test */
        CopyBigInt(&pMinusOne, &tmp_pMinusOne);
        for(j = 0; j < miller_rabin_max_test_time; j++)
        {
            DoDiv(&tmp_pMinusOne, &two, &tmp_pMinusOne, &remainder);

            DoPowMod(&base, &tmp_pMinusOne, p, &result);

            if(MILLER_RABIN_DEBUG)
            {
                printf("x: ");
                BigIntToStr(&tmp_pMinusOne, tmp_decimal_big_int);
                for(z = 0; z < strlen(tmp_decimal_big_int); z++)
                {
                    printf("%c", tmp_decimal_big_int[z]);
                }
                printf("\n");

                printf("result: ");
                BigIntToStr(&result, tmp_decimal_big_int);
                for(z = 0; z < strlen(tmp_decimal_big_int); z++)
                {
                    printf("%c", tmp_decimal_big_int[z]);
                }
                printf("\n");
            }

            if(DoCompare(&result, &pMinusOne) != 0 && DoCompare(&result, &one) != 0)
            {
                /* not prime */
                return 0;
            }
        }
    }

    if(MILLER_RABIN_DEBUG)
    {
        printf("\n\n");
    }

    return 1;
}

int MillerRabin(char *source, int times)
{
    BigInt bi;
    StrToBigInt(source, &bi);

    return DoMillerRabin(&bi, times);
}

void DoGenPrime(int byteLen, int times, char *file_name)
{
    if(byteLen * BYTE_SIZE * 2 > BIG_INT_BIT_LEN)
    {
        printf("DoGenPrime, prime is too big %d\n", byteLen);
        exit(1);
    }

    printf("DoGenPrime begin ...\n");

    int i;
    char str_test_num[BIG_INT_BIT_LEN];
    BigInt one;
    StrToBigInt("1", &one);

    /*  */
    char prime_file_name[FILE_NAME_LEN];
    snprintf(prime_file_name, FILE_NAME_LEN, "%s.prime",  file_name);
    FILE *p_prime_file;
    p_prime_file = fopen(prime_file_name, "wt");
    if(p_prime_file == NULL)
    {
        printf("DoGenPrime, open file %s err\n", prime_file_name);
        exit(1);
    }

    /*  */
    BigInt minusTwo, result;
    StrToBigInt("-2", &minusTwo);

    /* max num of {byteLen}'s bytes length */
    memset(result.bit, 0, BIG_INT_BIT_LEN);
    for (i = 0; i < byteLen * BYTE_SIZE; i++)
    {
        result.bit[i] = 1;
    }
    result.bit[SIGN_BIT] = 0;

    /*  */
    while(DoCompare(&result, &one) > 0)
    {
        BigIntToStr(&result, str_test_num);
        printf("testing number %s ...\n", str_test_num);

        if(DoMillerRabin(&result, times))
        {
            /*  */
            printf("detect prime %s\n", str_test_num);

            /*  */
            if(EOF == fputs(str_test_num, p_prime_file) || EOF == fputc(',', p_prime_file))
            {
                printf("DoGenPrime, write prime to file err\n");
                exit(1);
            }
        }

        /*  */
        DoAdd(&result, &minusTwo, &result);
    }
    
    /*  */
    fclose(p_prime_file);
}

char* DoGenRandomPrime(int byteLen, int times, BigInt *result)
{
    if(byteLen * BYTE_SIZE * 2 > BIG_INT_BIT_LEN)
    {
        printf("DoGenRandomPrime, prime is too big %d\n", byteLen);
        exit(1);
    }

    unsigned long a, b;
    char str_test_num[BIG_INT_BIT_LEN];

    /*  */
    a = time(0);

    /*  */
    while(1)
    {
        DoGetPositiveOddRandBigInt(byteLen, result);

        BigIntToStr(result, str_test_num);

        if(MILLER_RABIN_DEBUG)
        {
            printf("testing number[%s]...\n", str_test_num);
        }
        
        if(DoMillerRabin(result, times))
        {
            break;
        }
    }

    /*  */
    b = time(0);
    printf("consume %d seconds\n", b - a);

    return result;
}