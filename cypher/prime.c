// #include "../common.h"
// #include <stdio.h>

// #define COMMON_PRIME_SIZE 34
// static int common_prime[COMMON_PRIME_SIZE] = {
//     251, 239, 227, 223, 211, 199, 191, 179, 
//     173, 167, 163, 151, 139, 131, 127, 107, 
//     103, 83, 79, 71, 67, 59, 47, 43, 
//     31, 23, 19, 17, 13, 11, 7, 5, 
//     3, 2
// };

// /*

// 费马小定理：对于素数p和任意整数a，有a ^ (p-1) = 1 (mod p)（同余）。反过来，满足a ^ (p-1) = 1 (mod p)，p也几乎一定是素数。

// 伪素数：如果n是一个正整数，如果存在和p互素的正整数a满足a ^ (p-1) = 1 (mod p)，我们说n是基于a的伪素数。如果一个数是伪素数，那么它几乎肯定是素数。

// Miller-Rabin测试：不断选取不超过p-1的基a(s次)，计算是否每次都有a ^ (p-1) = 1 (mod p)，若每次都成立则p是素数，否则为合数。如果p是素数，x是小于p的正整数，且 x^2 = 1(mod p)，那么要么x=1，要么x=p-1;

// 模运算与基本四则运算有些相似，但是除法例外。其规则如下：
// (a + b) % p = (a % p + b % p) % p
// (a - b) % p = (a % p - b % p) % p
// (a * b) % p = (a % p * b % p) % p
// (a^b) % p = ((a % p)^b) % p

// 推论：
// 若a≡b (% p)，则对于任意的c，都有(a + c) ≡ (b + c) (%p)；
// 若a≡b (% p)，则对于任意的c，都有(a * c) ≡ (b * c) (%p)；
// 若a≡b (% p)，c≡d (% p)，则 (a + c) ≡ (b + d) (%p)，(a - c) ≡ (b - d) (%p)，
// (a * c) ≡ (b * d) (%p)，(a / c) ≡ (b / d) (%p)； 

// */
// int DoMillerRabin(const BigInt *const p, const int times)
// {
//     int i, j, z, miller_rabin_max_test_time;
//     BigInt result, base;
//     BigInt one, two, pMinusOne, tmp_pMinusOne, remainder;
//     char s_p[BIG_INT_BIT_LEN];
//     char s_prime[BIG_INT_BIT_LEN];

//     /*  */
//     BigIntToStr(p, s_p);
//     for(i = 0; i < COMMON_PRIME_SIZE; i++)
//     {
//         snprintf(s_prime, BIG_INT_BIT_LEN, "%d", common_prime[i]);
//         if(strcmp(s_p, s_prime) == 0)
//         {
//             return 1;
//         }
//     }   

//     char tmp_decimal_big_int[BIG_INT_BIT_LEN];

//     StrToBigInt("1", &one);
//     StrToBigInt("2", &two);
//     DoSub(p, &one, &pMinusOne);

//     /* Get Max Miller-Rabin test times */
//     miller_rabin_max_test_time = GetMaxRightShiftLen(&pMinusOne);

//     /*  */
//     if(MILLER_RABIN_DEBUG)
//     {
//         printf("\nDoMillerRabin, p: ");
//         BigIntToStr(p, tmp_decimal_big_int);
//         for(j = 0; j < strlen(tmp_decimal_big_int); j++)
//         {
//             printf("%c", tmp_decimal_big_int[j]);
//         }
//         printf("\n");
//     }

//     /* do {times} test, {times} is bigger, more accuracy */
//     for(i = 0; i < times; i++)
//     {
//         do
//         {
//             DoGetPositiveRand(p, &base);
//         }
//         while(DoCompare(&base, &two) < 0);
        
//         if(MILLER_RABIN_DEBUG)
//         {
//             printf("DoMillerRabin, base: ");
//             BigIntToStr(&base, tmp_decimal_big_int);
//             for(j = 0; j < strlen(tmp_decimal_big_int); j++)
//             {
//                 printf("%c", tmp_decimal_big_int[j]);
//             }
//             printf("\n");
//         }

//         /* Fermat test */
//         DoPowMod(&base, &pMinusOne, p, &result); /* result = base ^ (p - 1) % p */

//         if(DoCompare(&result, &one) != 0)
//         {
//             /* not prime */
//             return 0;
//         }
       
//         /* Miller-Rabin test */
//         CopyBigInt(&pMinusOne, &tmp_pMinusOne);
//         for(j = 0; j < miller_rabin_max_test_time; j++)
//         {
//             DoDiv(&tmp_pMinusOne, &two, &tmp_pMinusOne, &remainder);

//             DoPowMod(&base, &tmp_pMinusOne, p, &result);

//             if(MILLER_RABIN_DEBUG)
//             {
//                 printf("DoMillerRabin, x: ");
//                 BigIntToStr(&tmp_pMinusOne, tmp_decimal_big_int);
//                 for(z = 0; z < strlen(tmp_decimal_big_int); z++)
//                 {
//                     printf("%c", tmp_decimal_big_int[z]);
//                 }
//                 printf("\n");

//                 printf("DoMillerRabin, result: ");
//                 BigIntToStr(&result, tmp_decimal_big_int);
//                 for(z = 0; z < strlen(tmp_decimal_big_int); z++)
//                 {
//                     printf("%c", tmp_decimal_big_int[z]);
//                 }
//                 printf("\n");
//             }

//             if(DoCompare(&result, &pMinusOne) != 0 && DoCompare(&result, &one) != 0)
//             {
//                 /* not prime */
//                 return 0;
//             }
//         }
//     }

//     if(MILLER_RABIN_DEBUG)
//     {
//         printf("\n\n");
//     }

//     return 1;
// }

// int MillerRabin(const char *const source, const int times)
// {
//     BigInt bi;
//     StrToBigInt(source, &bi);

//     return DoMillerRabin(&bi, times);
// }

// void DoGenPrime(const int byteLen, const int times, const char *const file_name)
// {
//     if(byteLen * BYTE_SIZE * 2 > BIG_INT_BIT_LEN)
//     {
//         printf("DoGenPrime, prime is too big %d\n", byteLen);
//         exit(1);
//     }

//     printf("DoGenPrime begin ...\n");

//     int i;
//     char str_test_num[BIG_INT_BIT_LEN];
//     BigInt one;
//     StrToBigInt("1", &one);

//     /*  */
//     char prime_file_name[FILE_NAME_LEN];
//     snprintf(prime_file_name, FILE_NAME_LEN, "%s.prime",  file_name);
//     FILE *p_prime_file;
//     p_prime_file = fopen(prime_file_name, "wt");
//     if(p_prime_file == NULL)
//     {
//         printf("DoGenPrime, open file %s err\n", prime_file_name);
//         exit(1);
//     }

//     /*  */
//     BigInt minusTwo, result;
//     StrToBigInt("-2", &minusTwo);

//     /* max num of {byteLen}'s bytes length */
//     memset(result.bit, 0, BIG_INT_BIT_LEN);
//     for (i = 0; i < byteLen * BYTE_SIZE; i++)
//     {
//         result.bit[i] = 1;
//     }
//     result.bit[SIGN_BIT] = 0;

//     /*  */
//     while(DoCompare(&result, &one) > 0)
//     {
//         BigIntToStr(&result, str_test_num);
//         printf("testing number %s ...\n", str_test_num);

//         if(DoMillerRabin(&result, times))
//         {
//             /*  */
//             printf("detect prime %s\n", str_test_num);

//             /*  */
//             if(EOF == fputs(str_test_num, p_prime_file) || EOF == fputc(',', p_prime_file))
//             {
//                 printf("DoGenPrime, write prime to file err\n");
//                 exit(1);
//             }
//         }

//         /*  */
//         DoAdd(&result, &minusTwo, &result);
//     }
    
//     /*  */
//     fclose(p_prime_file);
// }

// char* DoGenRandomPrime(const int if_fix_len, const int byteLen, const int times, BigInt *result)
// {
//     if(byteLen * BYTE_SIZE * 2 > BIG_INT_BIT_LEN)
//     {
//         printf("DoGenRandomPrime, prime is too big %d\n", byteLen);
//         exit(1);
//     }

//     unsigned long a, b;
//     char str_test_num[BIG_INT_BIT_LEN];

//     /*  */
//     a = time(0);

//     /*  */
//     while(1)
//     {
//         DoGetPositiveOddRandBigInt(if_fix_len, byteLen, result);

//         BigIntToStr(result, str_test_num);

//         if(MILLER_RABIN_DEBUG)
//         {
//             printf("testing number[%s]...\n", str_test_num);
//         }
        
//         if(DoMillerRabin(result, times))
//         {
//             break;
//         }
//     }

//     /*  */
//     b = time(0);
//     printf("consume %d seconds\n", b - a);

//     return result;
// }