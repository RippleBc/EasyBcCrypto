#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

static int source_index = 0;
static unsigned char source[MAX_STR_SIZE];

int main()
{

    int i;
    char random[BIG_INT_BIT_LEN];

    GetOddRandBigInt(BIG_INT_BIT_LEN / 8, random);
    for(i = 0; i < strlen(random); i++)
    {
        printf("%c", random[i]);
    }
    printf("\n");

    return 0;
}