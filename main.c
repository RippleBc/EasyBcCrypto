#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

static int source_index = 0;
static unsigned char source[MAX_STR_SIZE];

int main()
{
    /*  */
    // FILE *codfp = fopen("./test/text", "r");
    // int c;
    // while((c = fgetc(codfp)) != EOF)
    // {
    //     source[source_index++] = (unsigned char)c;
    // }

    // int i, j;
    // int size = source_index;
    
    // unsigned char random[128];
    // get_specified_size_random(source, random, 128);


    unsigned char str[BIG_INT_BIT_LEN];

    Number n;
    BigInt bi;
    BigInt result;
    int i;

    // StrToNumber("-2048a1024a512a256a128", &n);
    // PrintNumber(&n);
    // NumberToStr(&n, str);
    // for(i = 0; i < strnlen(str, BIG_INT_BIT_LEN); i++)
    // {
    //     printf("%c", str[i]);
    // }
    // printf("\n");

    // StrToNumber("2048a1024a512a256a128", &n);
    // PrintNumber(&n);
    // NumberToStr(&n, str);
    // for(i = 0; i < strnlen(str, BIG_INT_BIT_LEN); i++)
    // {
    //     printf("%c", str[i]);
    // }
    // printf("\n");

    // StrToNumber("+2048a1024a512a256a128", &n);
    // PrintNumber(&n);
    // NumberToStr(&n, str);
    // for(i = 0; i < strnlen(str, BIG_INT_BIT_LEN); i++)
    // {
    //     printf("%c", str[i]);
    // }
    // printf("\n");

    StrToBigInt("-8", &bi);
    // PrintBigInt(&bi);

    // DoMul(&bi, &bi, &result);
    // StrToNumber("-8", &n);
    // PrintNumber(&n);

    // DoAdd(&bi, &bi, &result);
    // PrintBigIntTureForm(&result);

    // DoMul(&bi, &bi, &result);
    // PrintBigIntTureForm(&result);

    BigInt remainder;
    DoDiv(&bi, &bi, &result, &remainder);
    PrintBigIntTureForm(&result);

    return 0;
}