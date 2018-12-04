#include "../common.h"

BigInt* DoGetOddRandBigInt(int byteLen, BigInt* result)
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
        for(i = 0; i < byteLen; i ++)
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

    return result;
}

char* GetOddRandBigInt(int byteLen, char* result)
{
    BigInt a;

    DoGetOddRandBigInt(byteLen, &a);

    return BigIntToStr(&a, result);
}

BigInt* DoGetRand(BigInt *n, BigInt *result)
{
    /*  */
    BigInt one;
    StrToBigInt("1", &one);
    if(DoCompare(&one, n) == 0)
    {
        printf("DoGetRand, and thing mod 1 is zero\n");
        exit(1);
    }

    /*  */
    int i;
    BigInt t, n_tmp;
    int random_byte_sequence_size = floor(BIG_INT_BIT_LEN / BYTE_SIZE);
    memset(result->bit, 0, random_byte_sequence_size);
    
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
    byteSequenceToBinBigInt(random_byte_sequence, random_byte_sequence_size, &t);

    /* convert to positive integer */
    t.bit[SIGN_BIT] = 0;

    /*  */
    CopyBigInt(n, &n_tmp);
    DoMod(&t, n, result);
   
    if (IsZero(result))
    {
        DoGetRand(&n_tmp, result);
    }

    return result;
}

/*

费马小定理：对于素数p和任意整数a，有a^p ≡ a (mod p)（同余）。反过来，满足a^p = a (mod p)，p也几乎一定是素数。

伪素数：如果n是一个正整数，如果存在和p互素的正整数a满足a ^ (p-1) = 1 (mod p)，我们说n是基于a的伪素数。如果一个数是伪素数，那么它几乎肯定是素数。

Miller-Rabin测试：不断选取不超过p-1的基b(s次)，计算是否每次都有b ^ (p-1) = 1 (mod p)，若每次都成立则p是素数，否则为合数。　 

如果p是素数，x是小于p的正整数，且 x^2 = 1(mod p)，那么要么x=1，要么x=p-1;

*/
static int DoMillerRabin(BigInt *p, int times)
{
    int i, j, s;
    BigInt a, x;
    BigInt one, two, nMinusOne;

    char tmp_decimal_big_int[BIG_INT_BIT_LEN];

    StrToBigInt("1", &one);
    StrToBigInt("2", &two);
    DoSub(p, &one, &nMinusOne);

    if(MILLER_RABIN)
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
        DoGetRand(p, &x);
        // PrintBigIntTrueForm(&x);
        if(MILLER_RABIN)
        {
            printf("x: ");
            BigIntToStr(&x, tmp_decimal_big_int);
            for(j = 0; j < strlen(tmp_decimal_big_int); j++)
            {
                printf("%c", tmp_decimal_big_int[j]);
            }
            printf("\n");
        }

        DoPowMod(&x, &nMinusOne, p, &a); /* a = x ^ (p - 1) % p */

        if (DoCompare(&a, &one) == 0)
        {
            continue;
        }

        printf("\n\n");
        return 0;
        // for (j = 0; j < s; j++)
        // {
        //     if (DoCompare(&a, &nMinusOne) == 0)
        //         goto LOOP;

        //     DoPowMod(&a, &two, p, &a); /* a = a^2 % n */
        // }

        // return 0;

        // LOOP:;
    }

    printf("\n\n");
    return 1;
}

BigInt* DoGenPrime(int byteLen, int times, BigInt *result)
{
    if(byteLen * BYTE_SIZE > BIG_INT_BIT_LEN)
    {
        printf("DoGenPrime, prime is too big %d\n", byteLen);
        exit(1);
    }

    int i;
    unsigned long a, b;
    char str_test_num[MAX_STR_SIZE];

    /*  */
    BigInt minusTwo;
    StrToBigInt("-2", &minusTwo);

    /* max num of {byteLen}'s bytes length */
    memset(result->bit, 0, BIG_INT_BIT_LEN);
    for (i = 0; i < byteLen * BYTE_SIZE; i++)
    {
        result->bit[i] = 1;
    }

    /*  */
    a = time(0);

    /*  */
    while(1)
    {
        BigIntToStr(result, str_test_num);
        printf("testing number[%s]...\n", str_test_num);
        

        if(DoMillerRabin(result, times))
        {
            /*  */
            b = time(0);
            printf("****************************finish test number[%s] (t=%lds)****************************\n\n", str_test_num, b - a);
        }

        /*  */
        DoAdd(result, &minusTwo, result);
    }

    return result;
}

char* GenPrime(int byteLen, int times, char *result)
{
    BigInt n;

    DoGenPrime(byteLen, times, &n);

    return BigIntToStr(&n, result);
}
