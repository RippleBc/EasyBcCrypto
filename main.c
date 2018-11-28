#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

static int source_index = 0;
static unsigned char source[MAX_STR_SIZE];

int main()
{

    /*  */
    FILE *codfp = fopen("./text", "r");
    int c;
    while((c = fgetc(codfp)) != EOF)
    {
        source[source_index++] = (unsigned char)c;
    }

    int i;
    for(i = 0; i < source_index; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");
    aes_cbc_encrypt(source);
    for(i = 0; i < source_index; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");

    aes_cbc_decrypt(source);
    for(i = 0; i < source_index; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");
}