#include "../common.h"

// 获取指定位数的奇数BigInt
BigInt* DoGetOddRandBigInt(int bitLen, BigInt* result)
{
    int i;
    static unsigned long seed = 0;

    memset(result->bit, 0, BIG_INT_BIT_LEN);
    result->bit[0] = 1;

    srand(time(0) + seed++);
    for (i = 1; i < bitLen - 1; i++)
    {
        result->bit[i] = rand() % 2;
    }

    result->bit[i] = 1;

    return result;
}

char* GetOddRandBigInt(int bitLen, char* result)
{
    BigInt a;

    DoGetOddRandBigInt(bitLen, &a);

    return BigIntToStr(&a, result);
}

// 随机获取小于n的正整数
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

char* GetRand(char* s, char* result)
{
    BigInt a, b;

    StrToBigInt(s, &a);
    DoGetRand(&a, &b);

    return BigIntToStr(&b, result);
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
