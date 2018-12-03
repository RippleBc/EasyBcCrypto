#include "./common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

static char* BinStrToHexStr(char* binStr, char* hexStr)
{
    int i, j, t;
    Number binNum;
    Number hexNum;

    StrToNumber(binStr, &binNum);

    /*  */
    hexNum.sign = binNum.sign;

    /*  */
    hexNum.len = (int)ceil(binNum.len / 4.0);

    /*  */
    for(i = 0; i < hexNum.len; i++)
    {
        j = 4 * i;

        /* group's first bit */
        t = binNum.value[j];

        /* group's second bit */
        if (j + 1 < binNum.len)
        {
            t += 2 * binNum.value[j + 1];
        }

        /* group's third bit */
        if (j + 2 < binNum.len)
        {
            t += 4 * binNum.value[j + 2];
        }

        /* group's fourth bit */
        if (j + 3 < binNum.len)
        {
            t += 8 * binNum.value[j + 3];
        }

        hexNum.value[i] = t;
    }

    return NumberToStr(&hexNum, hexStr);
}

static BigInt* ToComplement(BigInt *src, BigInt *dst)
{
    int i;

    if(src->bit[SIGN_BIT] == NEGATIVE)
    {
        dst->bit[SIGN_BIT] = NEGATIVE;

        for(i = 0; i < SIGN_BIT && src->bit[i] == 0; i++)
        {
            dst->bit[i] = src->bit[i];
        }

        if(i == SIGN_BIT)
        {
            /* translate -0 to +0 */
            dst->bit[i] = 0; 
        }
        else
        {
            /* carray */
            dst->bit[i] = src->bit[i];

            /* opposite */
            for(i++; i < SIGN_BIT; i++)
            {
                dst->bit[i] = !src->bit[i];
            }
        }
    }
    else
    {
        /* positive, remain unchange  */
        for(i = 0; i < BIG_INT_BIT_LEN; i++)
        {
            dst->bit[i] = src->bit[i];
        }
    }

    return dst;
}

static BigInt* ToTrueForm(BigInt* src, BigInt* dst)
{
    return ToComplement(src, dst);
}

static BigInt* ToOppositeNumberComplement(BigInt *src, BigInt *dst)
{
    int i;

    for (i = 0; i < BIG_INT_BIT_LEN && src->bit[i] == 0; i++)
    {
        dst->bit[i] = src->bit[i];
    }

    /* notice, if big num is 10000...000 = 2 ^ (BIG_INT_BIT_LEN - 1), it has no opposite big int */
    if(i == SIGN_BIT)
    {
        printf("min bit int has no opposite bit int\n");
        exit(1);
    }

    if(i != BIG_INT_BIT_LEN)
    {
        dst->bit[i] = src->bit[i];

        for(i++; i < BIG_INT_BIT_LEN; i++)
        {
            dst->bit[i] = !src->bit[i];
        }
    }
    return dst;
}

static BigInt* BinNumToBigInt(Number *binNum, BigInt *a)
{
    if(binNum->len > SIGN_BIT)
    {
        char s_decimal[NUMBER_BIT_LEN];
        char s_binNum[NUMBER_BIT_LEN];

        /*  */
        NumberToStr(binNum, s_binNum);

        ChangeStringRadix(s_binNum, 2, 10, s_decimal);

        /*  */
        printf("BinNumToBigInt, number is too big, %s\n", s_decimal);
        exit(1);
    }

    int i;

    memset(a->bit, 0, BIG_INT_BIT_LEN);

    /*  */
    for(i = 0; i < binNum->len; i++)
    {
        a->bit[i] = binNum->value[i];
    }

    if(binNum->sign == POSITIVE)
    {
        return a;
    }

    a->bit[SIGN_BIT] = NEGATIVE;

    return ToComplement(a, a);
}

static Number* BigIntToBinNum(BigInt *a, Number *binNum)
{
    int i;
    BigInt t;

    binNum->sign = a->bit[SIGN_BIT];

    for(i = SIGN_BIT - 1; i >= 0 && a->bit[i] == 0; i--)
        ;

    /* min number, 11111...111 */
    if(binNum->sign == NEGATIVE && i == -1)
    {
        binNum->len = NUMBER_BIT_LEN;
        for(i = 0; i < binNum->len; i++)
        {
            binNum->value[i] = 1;
        }
    }
    else
    {
        ToTrueForm(a, &t);

        /* skip top zero */
        for(i = SIGN_BIT - 1; i >= 0 && t.bit[i] == 0; i--)
            ;

        /*  */
        binNum->len = i == -1 ? 1 : i + 1;

        /*  */
        for(i = 0; i < binNum->len; i++)
        {
            binNum->value[i] = t.bit[i];
        }
    }

    return binNum;
}

static int GetTrueValueLen(BigInt* a)
{
    int i;
    BigInt t;

    ToTrueForm(a, &t);

    for(i = SIGN_BIT - 1; i >= 0 && t.bit[i] == 0; i--)
        ;

    return i + 1;
}

char* ChangeStringRadix(char *str, int srcBase, int dstBase, char *resultStr)
{
    if(srcBase != 2 && srcBase != 8 && srcBase != 10 && srcBase != 16)
    {
        printf("ChangeStringRadix, srcBase only support 2, 8, 10, 16 system, %d\n", srcBase);
        exit(1);
    }

    if(dstBase != 2 && dstBase != 8 && dstBase != 10 && dstBase != 16)
    {
        printf("ChangeStringRadix, dstBase only support 2, 8, 10, 16 system, %d\n", dstBase);
        exit(1);
    }

    if(srcBase < dstBase)
    {
        char hexStr[BUFFER_SIZE];

        /* translate to binary format */
        ChangeStringRadix(str, srcBase, 2, resultStr);

        /* translate from binary str to hex str */
        BinStrToHexStr(resultStr, hexStr);

        /* translate from hex str to dstBase str */
        return ChangeStringRadix(hexStr, 16, dstBase, resultStr);
    }

    if(srcBase == dstBase)
    {
        return strcpy(resultStr, str);
    }

    else
    {
        int i, t;
        Number dividend; /* 被除数 */
        Number quotient; /* 商 */
        Number resultNum; /* 结果 */

        /*  */
        StrToNumber(str, &dividend);

        resultNum.len = 0;
        resultNum.sign = dividend.sign;

        while(dividend.len > 0)
        {
            quotient.len = dividend.len;

            /* 模拟人做除法的方式, 即一轮(求一位余数)的过程 */
            for(t = 0, i = dividend.len - 1; i >= 0; i--)
            {
                /* translate to decimal */
                t = t * srcBase + dividend.value[i];

                /*  */
                quotient.value[i] = t / dstBase;

                /*  */
                t = t % dstBase;
            }

            if(resultNum.len >= NUMBER_BIT_LEN)
            {
                printf("ChangeStringRadix, str num is too big %s\n", str);
                exit(1);
            }

            /* 保存一轮的结果, 即一位余数 */
            resultNum.value[resultNum.len++] = t;

            /* filter the zero at front of the quotient */
            for(i = quotient.len - 1; i >= 0 && quotient.value[i] == 0; i--)
                ;

            dividend.len = i + 1;

            /* 把商作为下一轮的被除数 */
            for(i = 0; i < dividend.len; i++)
            {
                dividend.value[i] = quotient.value[i];
            }
        }

        return NumberToStr(&resultNum, resultStr);
    }
}

BigInt* CopyBigInt(BigInt *src, BigInt *dst)
{
    int i;
    for(i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        dst->bit[i] = src->bit[i];
    }
    return dst;
}

BigInt* ShiftArithmeticLeft(BigInt *src, int indent, BigInt *dst)
{
    int i, j;

    dst->bit[SIGN_BIT] = src->bit[SIGN_BIT];

    for(i = SIGN_BIT - 1, j = i - indent; j >= 0; i--, j--)
    {
        dst->bit[i] = src->bit[j];
    }

    while(i >= 0)
    {
        dst->bit[i--] = 0;
    }

    return dst;
}

BigInt* ShiftArithmeticRight(BigInt* src, int indent, BigInt* dst)
{
    int i, j;

    dst->bit[SIGN_BIT] = src->bit[SIGN_BIT];

    for(i = 0, j = i + indent; j < SIGN_BIT; i++, j++)
    {
        dst->bit[i] = src->bit[j];
    }

    /* sign bit extend */
    while(i < SIGN_BIT)
    {
        dst->bit[i++] = src->bit[SIGN_BIT];
    }

    return dst;
}

BigInt* DoAdd(BigInt* a, BigInt* b, BigInt* result)
{
    int i, t, carryFlag;

    int aSign = a->bit[SIGN_BIT];
    int bSign = b->bit[SIGN_BIT];

    for(carryFlag = i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        /*  */
        t = a->bit[i] + b->bit[i] + carryFlag;

        /*  */
        result->bit[i] = t % 2;

        /*  */
        carryFlag = t > 1 ? 1 : 0;
    }

    if(aSign == bSign && aSign != result->bit[SIGN_BIT])
    {
        printf("DoAdd, overflow\n");
        exit(1);
    }

    return result;
}

BigInt* DoSub(BigInt* a, BigInt* b, BigInt* result)
{
    BigInt t;

    ToOppositeNumberComplement(b, &t);
    DoAdd(a, &t, result);

    return result;
}

/* Booth algorithm */
BigInt* DoMul(BigInt* a, BigInt* b, BigInt* result)
{
    int i;
    BigInt c, t;

    /*  */
    ToOppositeNumberComplement(a, &c);

    memset(t.bit, 0, BIG_INT_BIT_LEN);

    for(i = SIGN_BIT; i > 0 && b->bit[i] == b->bit[i - 1]; i--)
        ;

    while(i > 0)
    {
        ShiftArithmeticLeft(&t, 1, &t);

        if(b->bit[i] != b->bit[i - 1])
        {
            DoAdd(&t, b->bit[i] < b->bit[i - 1] ? a : &c, &t);
        }

        i--;
    }

    /*  */
    ShiftArithmeticLeft(&t, 1, &t);
    if(b->bit[0] != 0)
    {
        DoAdd(&t, &c, &t);
    }

    return CopyBigInt(&t, result);
}

int GetMaxLeftShiftLen(BigInt* a)
{
    int i, k;
    BigInt t;

    ToTrueForm(a, &t);

    for(i = SIGN_BIT - 1, k = 0; i >= 0 && t.bit[i] == 0; i--, k++)
        ;

    return k;
}

int GetMaxRightShiftLen(BigInt* a)
{
    int i;

    for (i = 0; i < SIGN_BIT && a->bit[i] == 0; i++)
        ;

    return i;
}

int IsZero(BigInt* a)
{
    int i;
    for(i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        if(a->bit[i] != 0)
        {
            return 0;
        }
    }
    return 1;
}

BigInt* DoDiv(BigInt* a, BigInt* b, BigInt* result, BigInt* remainder)
{
    int low, high, mid;
    BigInt c, d, e, t;

    low = 0;
    high = GetMaxLeftShiftLen(b);
 
    memset(t.bit, 0, BIG_INT_BIT_LEN);
    CopyBigInt(a, &c);

    if(a->bit[SIGN_BIT] == b->bit[SIGN_BIT])
    {
        t.bit[SIGN_BIT] = POSITIVE;

        while(1)
        {
            while(low <= high)
            {
                mid = (low + high) / 2;

                /*  */
                ShiftArithmeticLeft(b, mid, &d);

                /*  */
                DoSub(&c, &d, &e);

                /* enough to sub */
                if(d.bit[SIGN_BIT] == e.bit[SIGN_BIT] || IsZero(&e))
                {
                    low = mid + 1;
                }
                else
                {
                    high = mid - 1;
                }
            }

            /* enough to sub */
            if(high != -1)
            {
                t.bit[high] = 1;

                /* sub */
                ShiftArithmeticLeft(b, high, &d);
                DoSub(&c, &d, &c);
                
                low = 0;
                high--;
            }
            else
            {
                /* not enough to sub */
                CopyBigInt(&c, remainder);
                break;
            }
        }
    }
    else
    {
        t.bit[SIGN_BIT] = NEGATIVE;

        while(1)
        {
            while(low <= high)
            {
                mid = (low + high) / 2;
                ShiftArithmeticLeft(b, mid, &d);
                DoAdd(&c, &d, &e);

                if(d.bit[SIGN_BIT] != e.bit[SIGN_BIT] || IsZero(&e))
                {
                    low = mid + 1;
                }
                else
                {
                    high = mid - 1;
                }
            }

            if(high != -1)
            {
                t.bit[high] = 1;

                ShiftArithmeticLeft(b, high, &d);
                DoAdd(&c, &d, &c);

                low = 0;
                high--;
            }
            else
            {
                CopyBigInt(&c, remainder);
                break;
            }
        }
    }

    return ToComplement(&t, result);
}

char* Add(char* s1, char* s2, char* result)
{
    BigInt a, b, c;
    
    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoAdd(&a, &b, &c);

    return BigIntToStr(&c, result);
}

char* Sub(char* s1, char* s2, char* result)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoSub(&a, &b, &c);
    
    return BigIntToStr(&c, result);
}

char* Mul(char* s1, char* s2, char* result)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoMul(&a, &b, &c);

    return BigIntToStr(&c, result);
}

char* Div(char* s1, char* s2, char* result, char* remainder)
{
    BigInt a, b, c, d;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoDiv(&a, &b, &c, &d);
    BigIntToStr(&d, remainder);

    return BigIntToStr(&c, result);
}

int DoCompare(BigInt* a, BigInt* b)
{
    BigInt c;

    DoSub(a, b, &c);

    if(IsZero(&c))
    {
        return 0;
    }
    if(c.bit[SIGN_BIT] == POSITIVE)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int Compare(char* s1, char* s2)
{
    BigInt a, b;

    StrToBigInt(s1, &a);
    StrToBigInt(s1, &b);

    return DoCompare(&a, &b);
}

BigInt* DoMod(BigInt* a, BigInt* b, BigInt* remainder)
{
    BigInt c;

    DoDiv(a, b, &c, remainder);

    return remainder;
}

char* Mod(char* s1, char* s2, char* remainder)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoMod(&a, &b, &c);

    return BigIntToStr(&c, remainder);
}

/* Notice, do not support negative pow operation */
BigInt* DoPow(BigInt* a, BigInt* b, BigInt* result)
{
    int i, len;
    BigInt t, buf;

    CopyBigInt(a, &buf);
    StrToBigInt("1", &t);
    len = GetTrueValueLen(b);

    for(i = 0; i < len; i++)
    {
        if(b->bit[i] == 1)
        {
            DoMul(&t, &buf, &t);
        }

        DoMul(&buf, &buf, &buf);
    }
    
    return CopyBigInt(&t, result);
}

char* Pow(char* s1, char* s2, char* result)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoPow(&a, &b, &c);

    return BigIntToStr(&c, result);
}

/* base on formula: (a * b) mod c = (a mod c) * (b mod c) mod c */
BigInt* DoPowMod(BigInt* a, BigInt* b, BigInt* c, BigInt* result)
{
    int i, len;
    unsigned long t1, t2;
    BigInt t, buf;

    printf("doing PowMod...\n");
    t1 = time(0);
    CopyBigInt(a, &buf);
    StrToBigInt("1", &t);
    len = GetTrueValueLen(b);

    for(i = 0; i < len; i++)
    {
        if(b->bit[i] == 1)
        {
            DoMul(&t, &buf, &t);
            DoMod(&t, c, &t);
        }

        DoMul(&buf, &buf, &buf);
        DoMod(&buf, c, &buf);
    }
    t2 = time(0);
    printf("finish PowMod (t=%lds)\n", t2 - t1);

    return CopyBigInt(&t, result);
}

char* PowMod(char* s1, char* s2, char* s3, char* result)
{
    BigInt a, b, c, d;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    StrToBigInt(s3, &c);
    DoPowMod(&a, &b, &c, &d);

    return BigIntToStr(&d, result);
}

Number* StrToNumber(char *str, Number *n)
{
    int i, j;

    if(str[0] == '-' || str[0] == '+')
    {
        /*  */
        n->len = strlen(str) - 1;
        if(n->len > NUMBER_BIT_LEN)
        {
            printf("StrToNumber, str is too long %s\n", str);
            exit(1);
        }

        /*  */
        n->sign = str[0] == '+' ? POSITIVE : NEGATIVE;

        for(i = 0, j = n->len; j > 0; j--, i++)
        {
            if(str[j] > '9')
            {
                n->value[i] = str[j] - 'a' + 10;
                if(str[j] > 'f')
                {
                    printf("StrToNumber invalid char, %c\n", str[j]);
                    exit(1);
                }
            }
            else
            {
                n->value[i] = str[j] - '0';
            }
        }
    }
    else
    {
        /*  */
        n->len = strlen(str);

        /*  */
        n->sign = POSITIVE;

        for(i = 0, j = n->len - 1; j >= 0; j--, i++)
        {
            if(str[j] > '9')
            {
                n->value[i] = str[j] - 'a' + 10;
                if(str[j] > 'f')
                {
                    printf("StrToNumber invalid char, %c\n", str[j]);
                    exit(1);
                }
            }
            else
            {
                n->value[i] = str[j] - '0';
            }
        }
    }

    return n;
}


char* NumberToStr(Number *n, char *str)
{
    int i = 0, j;

    if(n->sign == NEGATIVE)
    {
        str[i++] = '-';
    }

    for(j = n->len - 1; j >= 0; j--)
    {
        if(n->value[j] > 9)
        {
            if(n->value[j] > 0x0f)
            {
                printf("NumberToStr invalid number, index %d, val %d\n", j, n->value[j]);
                exit(1);
            }

            str[i++] = n->value[j] - 10 + 'a';
        }
        else
        {
            str[i++] = n->value[j] + '0';
        }
    }

    str[i] = '\0';

    return str;
}

BigInt* StrToBigInt(char *s, BigInt *a)
{
    char buf[BUFFER_SIZE];
    Number binNum;

    /*  */
    ChangeStringRadix(s, 10, 2, buf);

    /*  */
    StrToNumber(buf, &binNum);

    /*  */
    return BinNumToBigInt(&binNum, a);
}

char* BigIntToStr(BigInt *a, char *s)
{
    char buf[BUFFER_SIZE];
    Number binNum;

    /*  */
    BigIntToBinNum(a, &binNum);

    /*  */
    NumberToStr(&binNum, buf);

    /*  */
    return ChangeStringRadix(buf, 2, 10, s);
}

BigInt* byteSequenceToBinBigInt(unsigned char *source, int size, BigInt *bigint)
{
    int i;
    unsigned char tmp;
    for(i = 0; i < size; i++)
    {
        bigint->bit[i * BYTE_SIZE] = (source[i] & 0x80) > 0x00 ? 1 : 0;

        bigint->bit[i * BYTE_SIZE + 1] = (source[i] & 0x40) > 0x00 ? 1 : 0;

        bigint->bit[i * BYTE_SIZE + 2] = (source[i] & 0x20) > 0x00 ? 1 : 0;

        bigint->bit[i * BYTE_SIZE + 3] = (source[i] & 0x10) > 0x00 ? 1 : 0;

        bigint->bit[i * BYTE_SIZE + 4] = (source[i] & 0x08) > 0x00 ? 1 : 0;

        bigint->bit[i * BYTE_SIZE + 5] = (source[i] & 0x04) > 0x00 ? 1 : 0;

        bigint->bit[i * BYTE_SIZE + 6] = (source[i] & 0x02) > 0x00 ? 1 : 0;

        bigint->bit[i * BYTE_SIZE + 7] = (source[i] & 0x01) > 0x00 ? 1 : 0;
    }

    return bigint;
}

void PrintBigInt(BigInt *a)
{
    int i;
    for(i = SIGN_BIT; i >= 0; i--)
    {
        printf("%d", a->bit[i]);
    }
    printf("\n");
}

void PrintBigIntTrueForm(BigInt* a)
{
    int i;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT; i >= 0; i--)
    {
        printf("%d", t.bit[i]);
    }
    printf("\n");
}

void PrintNumber(Number *n)
{
    int i;

    if(n->sign == NEGATIVE)
    {
        printf("-");
    }

    for(i = n->len - 1; i >= 0; i--)
    {
        if(n->value[i] > 9)
        {
            printf("%c", n->value[i] - 10 + 'a');
        }
        else
        {
            printf("%d", n->value[i]);
        }
    }
    printf("\n");
}