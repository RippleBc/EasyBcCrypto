#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

static int source_index = 0;
static unsigned char source[MAX_STR_SIZE];

int main()
{
  // rsa();

  ecc();

  return 0;
}

void rsa()
{
  int i;
  
  // DoGenerateRsaKey(1, "key_pair");

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


  RsaSign("56", dest, "key_pair");
  for(i = 0; i < strlen(dest); i++)
  {
    printf("%c", dest[i]);
  }
  printf("\n");

  RsaVerifySign(dest, source, "key_pair");
  for(i = 0; i < strlen(source); i++)
  {
    printf("%c", source[i]);
  }
  printf("\n");
}

void ecc()
{
  init_domain_parameters();
  generate_ecc_key("key_pair");
}