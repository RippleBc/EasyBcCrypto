#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

static int source_index = 0;
static unsigned char source[MAX_STR_SIZE];

int main()
{

    int i, j;
   
   	BigInt e, d, n;

   	// DoGenPrime(1, 5, "255");

   	// StrToBigInt("5", &e);
   	// StrToBigInt("144", &n);
   	// GeneD(&e, &d, &n);
   	// printf("result: \n");
   	// unsigned char dest[MAX_STR_SIZE];
   	// BigIntToStr(&d, dest);
   	// for(i = 0; i < strlen(dest); i++)
   	// {
   	// 	printf("%c", dest[i]);
   	// }
   	// printf("\n");

    DoGenerateRsaKey(1, "key_pair");

    // char result[MAX_STR_SIZE];
    // Mul("-10", "1", result);
    // for(i = 0; i < strlen(result); i++)
    // {
    //   printf("%c", result[i]);
    // }
    // printf("\n");

   	unsigned char dest[MAX_STR_SIZE];

   	RsaEncrypt("56", dest, "key_pair");
   	for(i = 0; i < strlen(dest); i++)
   	{
   		printf("%c", dest[i]);
   	}
   	printf("\n");

   	RsaDecrypt(dest, source, "key_pair");
   	for(i = 0; i < strlen(source); i++)
   	{
   		printf("%c", source[i]);
   	}
   	printf("\n");
    return 0;
}