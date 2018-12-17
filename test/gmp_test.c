#include <gmp.h>

void main()
{
	mpz_t n;

	mpz_init (n);
	
	mpz_init_set_str(n, "123456789", 0);

	gmp_printf("%Zd\n", n);
}


// gcc gmp_test.c -lgmp