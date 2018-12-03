#include "../common.h"

#define BYTE_LEN 8

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
        printf("hex random result begin\n");
        for(i = 0; i < byteLen; i ++)
        {
            printf("%x ", random_byte_sequence[i]);
        }
        printf("\nhex random result end\n\n");
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

/*  */
BigInt* DoGetRand(BigInt* n, BigInt* result)
{
    int i;
    static unsigned seed = 0;
    BigInt t;

    srand(time(0) + seed++);

    for (i = 0; i < SIGN_BIT; i++)
        t.bit[i] = rand() % 2;

    t.bit[SIGN_BIT] = 0;

    DoMod(&t, n, &t);
    if (IsZero(&t))
        DoGetRand(n, &t);

    return CopyBigInt(&t, result);
}

int DoMillerRabin(BigInt* n, int times)
{
    int i, j, s;
    BigInt a, t, x;
    BigInt one, two, nMinusOne;

    StrToBigInt("1", &one);      // one = 1
    StrToBigInt("2", &two);
    DoSub(n, &one, &nMinusOne);  // nMinusOne = n - 1

    s = GetMaxRightShiftLen(&nMinusOne);      // 获取最大的右移长度
    ShiftArithmeticRight(&nMinusOne, s, &t);  // 右移并保存到t中

    for (i = 0; i < times; i++)    // 做times次测试
    {
        DoGetRand(n, &x);         // 获取小于n的随机数
        DoPowMod(&x, &t, n, &a);  // a = x^t % n, 这里时间用得长

        if (DoCompare(&a, &one) == 0)
            continue;

        for (j = 0; j < s; j++)
        {
            if (DoCompare(&a, &nMinusOne) == 0)
                goto LOOP;

            DoPowMod(&a, &two, n, &a);  // a = a^2 % n
        }

        /* 第二种写法
        if (DoCompare(&a, &one) == 0 || DoCompare(&a, &nMinusOne) == 0)
            continue;

        for (j = 0; j < s - 1; j++)
        {
            DoPowMod(&a, &two, n, &a);  // a = a^2 % n

            if (DoCompare(&a, &nMinusOne) == 0)
                goto LOOP;
        }
        */

        return 0;

        LOOP:;
    }

    // 返回n可能是素数
    return 1;
}

int MillerRabin(char* s, int times)
{
    BigInt n;

    StrToBigInt(s, &n);
    
    return DoMillerRabin(&n, times);
}

// 生成指定位数和MillerRabin测试次数的"素数"
BigInt* DoGenPrime(int bitLen, int times, BigInt* result)
{
    int i;
    unsigned long n = 1;
    unsigned long a, b;
    BigInt minusTwo;

    StrToBigInt("-2", &minusTwo);
    memset(result->bit, 0, BIG_INT_BIT_LEN);
    for (i = 0; i < bitLen; i++)
        result->bit[i] = 1;

    while (1)
    {
        printf("testing number[%ld]...\n", n);
        a = time(0);

        if (DoMillerRabin(result, times))
        {
            b = time(0);
            break;
        }

        b = time(0);
        printf("finish test number %ld (t=%lds)\n\n", n++, b - a);

        DoAdd(result, &minusTwo, result);
    }

    printf("finish test number[%ld] (t=%lds)\n\n", n, b - a);

    return result;
}

char* GenPrime(int bitLen, int times, char* result)
{
    BigInt n;

    DoGenPrime(bitLen, times, &n);

    return BigIntToStr(&n, result);
}
