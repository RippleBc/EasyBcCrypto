#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

static int source_index = 0;
static unsigned char source[MAX_STR_SIZE];

int main()
{

    int i, j;
   
   	BigInt e, d, n;

   	DoGenPrime(1, 5, "255");

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

    // DoGenerateRsaKey(1, "key_pair");

   	// StrToBigInt("1217", &e);
   	// StrToBigInt("8323", &d);
   	// StrToBigInt("34189", &n);

   	// unsigned char dest[MAX_STR_SIZE];

   	// RsaEncrypt("56", dest, &d, &n);
   	// for(i = 0; i < strlen(dest); i++)
   	// {
   	// 	printf("%c", dest[i]);
   	// }
   	// printf("\n");

   	// RsaDecrypt(dest, source, &e, &n);
   	// for(i = 0; i < strlen(source); i++)
   	// {
   	// 	printf("%c", source[i]);
   	// }
   	// printf("\n");
    return 0;
}