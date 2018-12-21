#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./common.h"
#include "./hash/sha256.h"

AC_CHECK_LIB(gmp, __gmpz_init);

unsigned char debug_tmp[MAX_STR_SIZE];

mpz_t G_BIG_INT_ZERO;
mpz_t G_BIG_INT_ONE;
mpz_t G_BIG_INT_TWO;
mpz_t G_BIG_INT_THREE;

void init()
{
  mpz_init_set_str(G_BIG_INT_ZERO, "0", 16);
  mpz_init_set_str(G_BIG_INT_ONE, "1", 16);
  mpz_init_set_str(G_BIG_INT_TWO, "2", 16);
  mpz_init_set_str(G_BIG_INT_THREE, "3", 16);

  InitRandom(8);
}

int main()
{
  if(TEST_BEGIN)
  {
    init();

    sha256_test();

    random_test();

    prime_test();

    rsa_test();

    ecc_test();
  }

  return 0;
}

void prime_test()
{
  mpz_t prime;

  mpz_init(prime);

  DoGenRandomPrime(1, 64, prime);

  gmp_printf("%Zd\n", prime);

  printf("\n\n\n\n");
}

void random_test()
{ 
  mpz_t random;
  mpz_t limit;

  mpz_init(random);

  mpz_init_set_str(limit, "123", 16);

  DoGetPositiveRandBigInt(0, 8, random);
  gmp_printf("%Zd\n", random);

  DoGetPositiveOddRandBigInt(1, 8, random);
  gmp_printf("%Zd\n", random);

  DoGetPositiveRand(limit, random);
  gmp_printf("%Zd\n", random);

  printf("\n\n\n\n");
}

void sha256_test()
{
  char *buf[] = {
      "",
      "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",

      "abc",
      "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",

      "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
      "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1",

      "The quick brown fox jumps over the lazy dog",
      "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592",

      "The quick brown fox jumps over the lazy cog", /* avalanche effect test */
      "e4c4d8f3bf76b692de791a173e05321150f7a345b46484fe427f6acc7ecc81be",

      "bhn5bjmoniertqea40wro2upyflkydsibsk8ylkmgbvwi420t44cq034eou1szc1k0mk46oeb7ktzmlxqkbte2sy",
      "9085df2f02e0cc455928d0f51b27b4bf1d9cd260a66ed1fda11b0a3ff5756d99"
  };

  uint8_t hash[SHA256_BYTES];
  size_t i, j;
  char hex_byte_sequence[SHA256_BYTES * 2];
  for (i = 0; i < (sizeof(buf) / sizeof(buf[0])); i += 2) 
  {
    /*  */
    sha256(buf[i], strlen(buf[i]), hash);
    for(j = 0; j < SHA256_BYTES; j++)
    {
      snprintf(&hex_byte_sequence[j * 2], 3, "%02x", hash[j]);
    }
    hex_byte_sequence[SHA256_BYTES * 2] = '\0';

    /*  */
    printf("verify: ", buf[i]);
    for (j = 0; j < SHA256_BYTES; j++)
    {
      if(buf[i + 1][j] != hex_byte_sequence[j])
      {
        printf("0\n");
        break;
      }
    } 
    if(j == SHA256_BYTES)
    {
      printf("1\n");
    }
  }

  printf("\n\n\n\n");
}

void rsa_test()
{
  int i;
  
  DoGenerateRsaKey(512, "key_pair");

  unsigned char origin[MAX_STR_SIZE];
  unsigned char dest[MAX_STR_SIZE];
  int dest_size, debug_tmp_size;

  for(i = 0; i < 256; i++)
  {
    origin[i] = 0x15;
  }

  RsaSign(origin, 256, dest, &dest_size, "key_pair");
  printf("RsaSign text: ");
  for(i = 0; i < dest_size; i++)
  {
    printf("%02x", dest[i]);
  }
  printf("\n");
  
  printf("RsaVerifySign: %d\n", RsaVerifySign(dest, dest_size, origin, 256, "key_pair"));

  printf("\n\n\n\n");
}

void ecc_test()
{
  int i;

  /*  */
  GenerateEccKey("key_pair");

  /* sha256 */
  char text[MAX_STR_SIZE] = "bhn5bjmoniertqea40wro2upyflkydsibsk8ylkmgbvwi420t44cq034eou1szc1k0mk46oeb7ktzmlxqkbte2sy";
  unsigned char hash[SHA256_BYTES];
  sha256(text, strnlen(text, MAX_STR_SIZE), hash);
  printf("EccVerifySign, sha256 result: ");
  for(i = 0; i < SHA256_BYTES; i++)
  {
    printf("%02x", hash[i]);
  }
  printf("\n");

  /* sign */
  unsigned char r[MAX_STR_SIZE];
  unsigned char s[MAX_STR_SIZE];
  int r_size, s_size;
  EccSign(hash, "key_pair", r, &r_size, s, &s_size);
  printf("r: ");
  for(i = 0; i < r_size; i++)
  {
    printf("%02x", r[i]);
  }
  printf("\n");
  printf("s: ");
  for(i = 0; i < s_size; i++)
  {
    printf("%02x", s[i]);
  }
  printf("\n");

  /* verify */
  printf("EccVerifySign: %d\n", EccVerifySign(hash, "key_pair", r, r_size, s, s_size));

  printf("\n\n\n\n");
}