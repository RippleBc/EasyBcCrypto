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
    int size = strlen(source);
    for(i = 0; i < size; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");

    /*  */
    aes_cbc_encrypt(source, size);
    for(i = 0; i < size; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");

    aes_cbc_decrypt(source, size);
    for(i = 0; i < size; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");

    /*  */
    aes_cfb_encrypt(source, size);
    for(i = 0; i < size; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");

    aes_cfb_decrypt(source, size);
    for(i = 0; i < size; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");

    /*  */
    aes_ofb_encrypt(source, size);
    for(i = 0; i < size; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");

    aes_ofb_decrypt(source, size);
    for(i = 0; i < size; i++)
    {
        printf("%c", source[i]);
    }
    printf("\n");
}