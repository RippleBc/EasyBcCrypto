#include "../common.h"
#include <stdio.h>

void DoGenRandomPrime(const int if_fix_len, const int byteLen, mpz_t prime)
{
	int prime_test_result;

	/*  */
	while(1)
	{
		DoGetPositiveOddRandBigInt(if_fix_len, byteLen, prime);

		if(PRIME_DEBUG)
		{
		    printf("DoGenRandomPrime, testing number ");
		    gmp_printf("%Zd\n", prime);
		}

		/*  */
		prime_test_result = mpz_probab_prime_p(prime, MILLER_RABIN_TIMES);
		if(prime_test_result == 1)
		{
			if(PRIME_DEBUG)
		  {
		      printf("DoGenRandomPrime, probably prime ");
		      gmp_printf("%Zd\n", prime);
		  }

		  break;
		}
		else if(prime_test_result == 2)
		{
			if(PRIME_DEBUG)
		  {
		      printf("DoGenRandomPrime, definitely prime ");
		      gmp_printf("%Zd\n", prime);
		  }
		  
			break;
		}
	}
}