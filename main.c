#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

static int source_index = 0;
static unsigned char source[MAX_STR_SIZE];

int main()
{
    /*  */
    FILE *codfp = fopen("./test/text", "r");
    int c;
    while((c = fgetc(codfp)) != EOF)
    {
        source[source_index++] = (unsigned char)c;
    }

    int i, j;
    int size = source_index;
    
    unsigned char random[128];
    get_specified_size_random(source, random, 128);
}