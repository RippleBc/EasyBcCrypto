/*
    miller-rabin 素数测试 (基于大整型)
    update: 2014.06.09 by xkuga
    
    函数: GenPrime(bitLen, times, result)
    可生成一个指定位长和miller-rabin测试次数的素数
    生成一个1024bit的素数大约需要6小时
    生成一个512bit的素数大约需要3小时
    生成一个100bit的素数大约需要1分钟
    生成一个32bit的素数大约需要4秒
    在生成一个大素前，请确保宏定义 BIG_INT_BIT_LEN 足够大

    注意:
    素数的生成不是随机的
    他是首先检测指定位长中最大的一个奇数，也就是全1的情况
    然后再每次减2，再测试，直到通过miller-rabin测试为止
    因此程序总是生成指定整型位数中最大的一个素数
    当然你也可以随机去生成素数，程序已经写了相关函数
    不过随机生成的话只能祝你好运了，而且我们一般需要更大的素数

    BigInt: 大整型（程序已实现）

    程序实现了这几种大数运算：+, -, *, /, ^, %, ^%
    采用的是二进制的方式实现, BigInt的存储与运算均采用补码
    乘法：采用的是Booth一位乘
    除法：用二分法去做减法
    幂运算与模幂运算：都是采用二进制的方式实现的，并非一个一个乘

    虽然可能不是很快，不过还是能用><
    而且程序比较易懂，对于理解计算机如何运算有一定的帮助
    如果有任何错误或改进可@C程序设计语言，谢谢!
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define BIG_INT_BIT_LEN 2400           // 定义BigInt的位数
#define SIGN_BIT BIG_INT_BIT_LEN - 1   // 符号位的位置
#define BUFFER_SIZE BIG_INT_BIT_LEN    // 缓冲区大小
#define POSITIVE 0                     // 0表示正数
#define NEGATIVE 1                     // 1表示负数

typedef struct    // 大整数类型，均用补码表示
{
    char bit[BIG_INT_BIT_LEN];
}BigInt;

typedef struct    // Number均用真值表示
{
    char value[BIG_INT_BIT_LEN];  // 数字数组
    int len;                      // 数组长度
    int sign;                     // 符号标记
}Number;

// 打印BigInt
void PrintBigInt(BigInt* a)
{
    int i;
    for (i = SIGN_BIT; i >= 0; i--)
        printf("%d", a->bit[i]);
    printf("\n");
}

// 打印Number
void PrintNumber(Number* n)
{
    int i;

    if (n->sign == NEGATIVE)
        printf("-");

    for (i = n->len - 1; i >= 0; i--)
    {
        if (n->value[i] > 9)  // 大于10进制的情况
            printf("%c", n->value[i] - 10 + 'a');
        else
            printf("%d", n->value[i]);
    }
    printf("\n");
}

// 把str转为Number数字类型, 返回Number*
Number* StrToNumber(char* str, Number* n)
{
    int i, j;

    if (str[0] == '-' || str[0] == '+')  // 0号单元存放符号
    {
        n->len = strlen(str) - 1;
        n->sign = str[0] == '+' ? POSITIVE : NEGATIVE;

        for (i = 0, j = n->len; j > 0; j--, i++)
        {
            if (str[j] > '9')  // 大于10进制的情况
                n->value[i] = str[j] - 'a' + 10;
            else
                n->value[i] = str[j] - '0';
        }
    }
    else
    {
        n->len = strlen(str);
        n->sign = POSITIVE;

        for (i = 0, j = n->len - 1; j >= 0; j--, i++)
        {
            if (str[j] > '9')  // 大于10进制的情况
                n->value[i] = str[j] - 'a' + 10;
            else
                n->value[i] = str[j] - '0';
        }
    }

    return n;
}

// Number类型转字符串类型
char* NumberToStr(Number* n, char* str)
{
    int i = 0, j;

    if (n->sign == NEGATIVE)
        str[i++] = '-';

    for (j = n->len - 1; j >= 0; j--)
    {
        if (n->value[j] > 9)  // 大于10进制的情况
            str[i++] = n->value[j] - 10 + 'a';
        else
            str[i++] = n->value[j] + '0';
    }

    str[i] = '\0';

    return str;
}

// 2进制字符串转16进制字符串
char* BinStrToHexStr(char* binStr, char* hexStr)
{
    int i, j, t;
    Number binNum;
    Number hexNum;

    StrToNumber(binStr, &binNum);

    hexNum.sign = binNum.sign;
    hexNum.len = (int)ceil(binNum.len / 4.0);

    for (i = 0; i < hexNum.len; i++)
    {
        j = 4 * i;

        t = binNum.value[j];

        if (j + 1 < binNum.len)
            t += 2 * binNum.value[j + 1];

        if (j + 2 < binNum.len)
            t += 4 * binNum.value[j + 2];

        if (j + 3 < binNum.len)
            t += 8 * binNum.value[j + 3];

        hexNum.value[i] = t;
    }

    return NumberToStr(&hexNum, hexStr);
}

// 字符串进制转换
char* ChangeStringRadix(char* str, int srcBase, int dstBase, char* resultStr)
{
    if (srcBase < dstBase)
    {
        char hexStr[BUFFER_SIZE];

        ChangeStringRadix(str, srcBase, 2, resultStr);
        BinStrToHexStr(resultStr, hexStr);

        return ChangeStringRadix(hexStr, 16, dstBase, resultStr);
    }

    if (srcBase == dstBase)
    {
        return strcpy(resultStr, str);
    }

    else
    {
        int i, t;
        Number dividend;   // 被除数
        Number quotient;   // 商
        Number resultNum;  // 结果

        // 把str转换为Number数字类型
        StrToNumber(str, &dividend);

        resultNum.len = 0;
        resultNum.sign = dividend.sign;

        while (dividend.len > 0)
        {
            quotient.len = dividend.len;

            // 模拟人做除法的方式, 即一轮(求一位余数)的过程
            for (t = 0, i = dividend.len - 1; i >= 0; i--)
            {
                t = t * srcBase + dividend.value[i];
                quotient.value[i] = t / dstBase;
                t = t % dstBase;      // 循环最后的t即为一轮的结果
            }

            // 保存一轮的结果, 即一位余数
            resultNum.value[resultNum.len++] = t;

            // 过滤商中多余的0
            for (i = quotient.len - 1; i >= 0 && quotient.value[i] == 0; i--);

            dividend.len = i + 1;

            // 把商作为下一轮的被除数
            for (i = 0; i < dividend.len; i++)
            {
                dividend.value[i] = quotient.value[i];
            }
        }

        return NumberToStr(&resultNum, resultStr);
    }
}

// 原码<=>补码
BigInt* ToComplement(BigInt* src, BigInt* dst)
{
    int i;

    if (src->bit[SIGN_BIT] == NEGATIVE)  // 负数求补
    {
        dst->bit[SIGN_BIT] = 1;

        for (i = 0; i < SIGN_BIT && src->bit[i] == 0; i++)
            dst->bit[i] = src->bit[i];

        if (i == SIGN_BIT)    // -0的补码
            dst->bit[i] = 0;
        else                  // 非0补码
        {
            dst->bit[i] = src->bit[i];
            for (i++; i < SIGN_BIT; i++)
                dst->bit[i] = !src->bit[i];
        }
    }
    else  // 正数求补不变
    {
        for (i = 0; i < BIG_INT_BIT_LEN; i++)
            dst->bit[i] = src->bit[i];
    }

    return dst;
}

// 转为原码
BigInt* ToTrueForm(BigInt* src, BigInt* dst)
{
    return ToComplement(src, dst);
}

// 原码打印BigInt
void PrintBigIntTureForm(BigInt* a)
{
    int i;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT; i >= 0; i--)
        printf("%d", t.bit[i]);
    printf("\n");
}

// 转为相反数的补码 [x]补 => [-x]补, 
// 注意：例如如果是8位整数，不能求-128相反数的补码
// 算法的思想是连同符号位一起求补，即符号位也要取反，可证明是正确的
BigInt* ToOppositeNumberComplement(BigInt* src, BigInt* dst)
{
    int i;

    for (i = 0; i < BIG_INT_BIT_LEN && src->bit[i] == 0; i++)
        dst->bit[i] = src->bit[i];

    // 求非0相反数的补码
    if (i != BIG_INT_BIT_LEN)
    {
        dst->bit[i] = src->bit[i];

        // 即符号位也要取反
        for (i++; i < BIG_INT_BIT_LEN; i++)
            dst->bit[i] = !src->bit[i];
    }

    return dst;
}

/* 基本实现
// 转为相反数的补码 [x]补 => [-x]补
BigInt* ToOppositeNumberComplement(BigInt* src, BigInt* dst)
{
    BigInt t;

    ToTrueForm(src, &t);
    t.bit[SIGN_BIT] = !t.bit[SIGN_BIT];
    ToComplement(&t, dst);

    return dst;
}
*/

// 2进制Number转BigInt
BigInt* BinNumToBigInt(Number* binNum, BigInt* a)
{
    int i;

    memset(a->bit, 0, BIG_INT_BIT_LEN);  // 初始化为0

    for (i = 0; i < binNum->len; i++)
    {
        a->bit[i] = binNum->value[i];
    }

    // 负数取下界的情况：如4位整型[1000]
    if (binNum->len == BIG_INT_BIT_LEN)
    {
        return a;
    }
    else
    {
        a->bit[SIGN_BIT] = binNum->sign;  // 符号位
        return ToComplement(a, a);
    }
}

// BigInt转2进制Number
Number* BigIntToBinNum(BigInt* a, Number* binNum)
{
    int i;
    BigInt t;

    binNum->sign = a->bit[SIGN_BIT];

    for (i = SIGN_BIT - 1; i >= 0 && a->bit[i] == 0; i--);

    // BigInt为负数的下界时
    if (binNum->sign == NEGATIVE && i == -1)
    {
        binNum->len = BIG_INT_BIT_LEN;
        for (i = 0; i < binNum->len; i++)
            binNum->value[i] = a->bit[i];
    }
    else
    {
        ToTrueForm(a, &t);
        for (i = SIGN_BIT - 1; i >= 0 && t.bit[i] == 0; i--);
        binNum->len = i == -1 ? 1 : i + 1;
        for (i = 0; i < binNum->len; i++)
            binNum->value[i] = t.bit[i];
    }

    return binNum;
}

// 字符串转BigInt，以补码存储
BigInt* StrToBigInt(char* s, BigInt* a)
{
    char buf[BUFFER_SIZE];
    Number binNum;

    ChangeStringRadix(s, 10, 2, buf);              // 十进制转二进制
    StrToNumber(buf, &binNum);           // string转Number
    return BinNumToBigInt(&binNum, a);   // Number转BigInt
}

// BigInt转字符串，以10进制表示
char* BigIntToStr(BigInt* a, char* s)
{
    char buf[BUFFER_SIZE];
    Number binNum;

    BigIntToBinNum(a, &binNum);     // BigInt转Number
    NumberToStr(&binNum, buf);      // Number转string

    return ChangeStringRadix(buf, 2, 10, s);  // 二进制转十进制
}

// 复制BigInt
BigInt* CopyBigInt(BigInt* src, BigInt* dst)
{
    int i;
    for (i = 0; i < BIG_INT_BIT_LEN; i++)
        dst->bit[i] = src->bit[i];
    return dst;
}

// 算术左移
BigInt* ShiftArithmeticLeft(BigInt* src, int indent, BigInt* dst)
{
    int i, j;

    dst->bit[SIGN_BIT] = src->bit[SIGN_BIT];

    for (i = SIGN_BIT - 1, j = i - indent; j >= 0; i--, j--)
    {
        dst->bit[i] = src->bit[j];
    }

    while (i >= 0)
    {
        dst->bit[i--] = 0;
    }

    return dst;
}

// 算术右移
BigInt* ShiftArithmeticRight(BigInt* src, int indent, BigInt* dst)
{
    int i, j;

    dst->bit[SIGN_BIT] = src->bit[SIGN_BIT];

    for (i = 0, j = i + indent; j < SIGN_BIT; i++, j++)
    {
        dst->bit[i] = src->bit[j];
    }

    while (i < SIGN_BIT)
    {
        dst->bit[i++] = src->bit[SIGN_BIT];
    }

    return dst;
}

// 加法实现
BigInt* DoAdd(BigInt* a, BigInt* b, BigInt* result)
{
    int i, t, carryFlag;           // 进位标志
    int aSign = a->bit[SIGN_BIT];  // a的符号
    int bSign = b->bit[SIGN_BIT];  // b的符号

    for (carryFlag = i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        t = a->bit[i] + b->bit[i] + carryFlag;
        result->bit[i] = t % 2;
        carryFlag = t > 1 ? 1 : 0;
    }

    if (aSign == bSign && aSign != result->bit[SIGN_BIT])
    {
        printf("Overflow XD\n");
        exit(1);
    }

    return result;
}

// 减法实现
BigInt* DoSub(BigInt* a, BigInt* b, BigInt* result)
{
    BigInt t;

    ToOppositeNumberComplement(b, &t);
    DoAdd(a, &t, result);

    return result;
}

// 乘法实现 Booth算法[补码1位乘] 转化为移位和加法
BigInt* DoMul(BigInt* a, BigInt* b, BigInt* result)
{
    int i;
    BigInt c, t;

    ToOppositeNumberComplement(a, &c);  // c=[-a]的补

    memset(t.bit, 0, BIG_INT_BIT_LEN);  // 初始化为0

    // 从高位处开始，过滤相同的位，因为相减为0
    for (i = SIGN_BIT; i > 0 && b->bit[i] == b->bit[i - 1]; i--);

    while (i > 0)
    {
        ShiftArithmeticLeft(&t, 1, &t);

        if (b->bit[i] != b->bit[i - 1])
        {
            DoAdd(&t, b->bit[i - 1] > b->bit[i] ? a : &c, &t);
        }

        i--;
    }

    // 最后一步的移位
    ShiftArithmeticLeft(&t, 1, &t);
    if (b->bit[0] != 0)
    {
        DoAdd(&t, &c, &t);
    }

    return CopyBigInt(&t, result);
}

// 在不溢出的情况下，获取最大算术左移的长度
int GetMaxLeftShiftLen(BigInt* a)
{
    int i, k;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT - 1, k = 0; i >= 0 && t.bit[i] == 0; i--, k++);

    return k;
}

// 在不溢出的情况下，获取最大算术右移的位数
int GetMaxRightShiftLen(BigInt* a)
{
    int i;

    for (i = 0; i < SIGN_BIT && a->bit[i] == 0; i++);

    return i;
}

// 判断Bigint是否为0
int IsZero(BigInt* a)
{
    int i;
    for (i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        if (a->bit[i] != 0)
            return 0;
    }
    return 1;
}

// 除法实现 用2分法去求商的各个为1的位 写得不够简洁><
BigInt* DoDiv(BigInt* a, BigInt* b, BigInt* result, BigInt* remainder)
{
    int low, high, mid;
    BigInt c, d, e, t;

    low = 0;                       // 初始化左移下限值
    high = GetMaxLeftShiftLen(b);  // 获取最大算术左移的长度

    memset(t.bit, 0, BIG_INT_BIT_LEN);  // 初始化商为0
    CopyBigInt(a, &c);                  // 初始化c为被除数a

    // 同号情况作减
    if (a->bit[SIGN_BIT] == b->bit[SIGN_BIT])
    {
        t.bit[SIGN_BIT] = POSITIVE;

        while (1)
        {
            while (low <= high)
            {
                mid = (low + high) / 2;
                ShiftArithmeticLeft(b, mid, &d);
                DoSub(&c, &d, &e);  // e = c - d

                // e >= 0，表示够减
                if (d.bit[SIGN_BIT] == e.bit[SIGN_BIT] || IsZero(&e))
                    low = mid + 1;
                else
                    high = mid - 1;
            }

            // high是最后够减的移位数
            // high == -1 表示已经连1倍的除数都不够减了
            if (high != -1)
            {
                t.bit[high] = 1;

                // 这里统一操作了，可改进
                ShiftArithmeticLeft(b, high, &d);
                DoSub(&c, &d, &c);  // c = c - d
                
                low = 0;
                high--;
            }
            else
            {
                // 这时c所表示的被除数即为最后的余数
                CopyBigInt(&c, remainder);
                break;
            }
        }
    }

    // 异号情况作加
    else
    {
        t.bit[SIGN_BIT] = NEGATIVE;

        while (1)
        {
            while (low <= high)
            {
                mid = (low + high) / 2;
                ShiftArithmeticLeft(b, mid, &d);
                DoAdd(&c, &d, &e);  // e = c + d

                // e >= 0
                if (d.bit[SIGN_BIT] != e.bit[SIGN_BIT] || IsZero(&e))
                    low = mid + 1;
                else
                    high = mid - 1;
            }

            // high是最后够减的移位数
            // high == -1 表示已经连1倍的除数都不够减了
            if (high != -1)
            {
                t.bit[high] = 1;

                // 这里统一操作了，可改进
                ShiftArithmeticLeft(b, high, &d);
                DoAdd(&c, &d, &c);  // c = c + d

                low = 0;
                high--;
            }
            else
            {
                // 这时c所表示的被除数即为最后的余数
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

// 比较两个BigInt的大小
int DoCompare(BigInt* a, BigInt* b)
{
    BigInt c;

    DoSub(a, b, &c);

    if (IsZero(&c))
        return 0;
    if (c.bit[SIGN_BIT] == POSITIVE)
        return 1;
    else
        return -1;
}

int Compare(char* s1, char* s2)
{
    BigInt a, b;

    StrToBigInt(s1, &a);
    StrToBigInt(s1, &b);

    return DoCompare(&a, &b);
}

// 求模实现
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

// 获取BigInt真值的位长度
int GetTrueValueLen(BigInt* a)
{
    int i;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT - 1; i >= 0 && t.bit[i] == 0; i--);

    return i + 1;
}

// 幂运算(二进制实现) 不能求负幂
BigInt* DoPow(BigInt* a, BigInt* b, BigInt* result)
{
    int i, len;
    BigInt t, buf;

    CopyBigInt(a, &buf);
    StrToBigInt("1", &t);
    len = GetTrueValueLen(b);  // 获取BigInt真值的位长度

    for (i = 0; i < len; i++)
    {
        if (b->bit[i] == 1)
            DoMul(&t, &buf, &t);  // t = t * buf

        // 这里最后多做了一次
        DoMul(&buf, &buf, &buf);  // buf = buf * buf
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

// 模幂运算(二进制实现)
BigInt* DoPowMod(BigInt* a, BigInt* b, BigInt* c, BigInt* result)
{
    int i, len;
    unsigned long t1, t2;
    BigInt t, buf;

    printf("    doing PowMod...\n");
    t1 = time(0);
    CopyBigInt(a, &buf);
    StrToBigInt("1", &t);
    len = GetTrueValueLen(b);  // 获取BigInt真值的位长度

    for (i = 0; i < len; i++)
    {
        if (b->bit[i] == 1)
        {
            DoMul(&t, &buf, &t);  // t = t * buf
            DoMod(&t, c, &t);     // t = t % c;
        }

        // 这里最后多做了一次
        DoMul(&buf, &buf, &buf);  // buf = buf * buf
        DoMod(&buf, c, &buf);     // buf = buf % c
    }
    t2 = time(0);
    printf("    finish PowMod (t=%lds)\n", t2 - t1);

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

int main()
{
    // printf("hello, world");

    unsigned long a, b;
    char result[BUFFER_SIZE];
    char remainder[BUFFER_SIZE];
    char buf[BUFFER_SIZE] = "prime=";

    // 函数: GenPrime(bitLen, times, result)
    // 可生成一个指定位长和miller-rabin测试次数的素数
    // 生成一个500bit的素数大约需要3小时
    // 生成一个100bit的素数大约需要1分钟
    // 生成一个32bit的素数大约需要4秒
    // 在生成一个大素前，请确保宏定义 BIG_INT_BIT_LEN 足够大

    a = time(0);
    puts(strcat(buf, GenPrime(32, 5, result)));  // 生成一个32位的素数
    b = time(0);
    printf("total t=%lds\n", b - a);
    
    /*
    // miller-rabin 素数测试
    a = time(0);
    printf("\n%s\n", MillerRabin("261", 5) ? "is prime" : "not prime");
    b = time(0);
    printf("total t=%lds\n", b - a);
    */
    
    // 一些素数的例子
    // 4567, 124567, 3214567, 23456789, 55566677
    // 1234567894987654321, 11111111111111111111111(23)
    // 779711611410512054554699111109

    // 100位中最大的素数
    // 1267650600228229401496703205361

    // 512位中最大的素数 t=13284s
    // 13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006083527

    // 1024位中最大的素数 t=20983s
    // 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137111

    // 大整型BigInt的调用例子
    //puts(Add("2010", "4", result));
    //puts(Sub("0", "2014", result));
    //puts(Mul("2", "43", result));
    //puts(Div("86", "10", result, remainder));
    //puts(remainder);
    //puts(Mod("-86", "10", result));
    //puts(PowMod("7", "80", "86", result));

    return 0;
}
