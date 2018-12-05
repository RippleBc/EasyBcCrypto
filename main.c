#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

static int source_index = 0;
static unsigned char source[MAX_STR_SIZE];

int main()
{

    int i, j;
   
   	BigInt e, d, n;

    DoGenerateRsaKey(&e, &d, &n, 3);

    

    return 0;
}