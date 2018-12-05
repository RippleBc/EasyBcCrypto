#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

static int source_index = 0;
static unsigned char source[MAX_STR_SIZE];

int main()
{

    int i, j;
    // char random[BIG_INT_BIT_LEN];

    // GetOddRandBigInt(BIG_INT_BIT_LEN / 8, random);
    // for(i = 0; i < strlen(random); i++)
    // {
    //     printf("%c", random[i]);
    // }
    // printf("\n");

		

		// char result[MAX_STR_SIZE];
		// char remainder[MAX_STR_SIZE];
		// Div("79846165497161654654984161231349749461313434679461313542414651681615674946134164", "56128", result, remainder);
		// for(i = 0; i < strlen(result); i++)
		// {
		// 	printf("%c", result[i]);
		// }
		// printf("\n");
		// for(i = 0; i < strlen(remainder); i++)
		// {
		// 	printf("%c", remainder[i]);
		// }
		// printf("\n");
		
		// char result[MAX_STR_SIZE];
		// BigInt a, b;
		// StrToBigInt("15", &a);
		// for(i = 0; i < 10; i++)
		// {
		// 	DoGetRand(&a, &b);
		// 	BigIntToStr(&b, result);
		// 	for(j = 0; j < strlen(result); j++)
		// 	{
		// 		printf("%c", result[j]);
		// 	}
		// 	printf("\n");
		// }
		

		// char prime_num[MAX_STR_SIZE];
		// DoGenPrime(1, 5, prime_num);

		// for(int i = 0; i < strlen(prime_num); i++)
		// {
		// 	printf("%c", prime_num[i]);
		// }

    DoGenerateRsaKey();

    return 0;
}