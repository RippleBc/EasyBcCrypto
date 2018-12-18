#include <gmp.h>

AC_CHECK_LIB(gmp, __gmpz_init);

void main()
{
	mpz_t a, b, quotient, remainder;
		
	// mpz_init_set_str (a, "0x9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999555555555555555555555555555555555555555555555222222222222222222222222222222222222222222222222222222222222222222222224444444444444444444444444411111111111111111111111111111111111111111111112222222222222222222222fffffffffffffffffffffffffffffffffffffffffffffffffbbbbbbbbbbbbbbbbbbbbb", 0);
	// mpz_init_set_str (b, "0xef", 0);
	// mpz_init(quotient);
	// mpz_init(remainder);
	// mpz_cdiv_qr_ui(quotient, remainder, a, b);
	// gmp_printf("%Zd %Zd\n", a, b);

	mpz_init_set_str(a, "1", 16);
	mpz_init_set_str(b, "6", 16);
	if(mpz_odd_p(b))
	{
		mpz_sub(b, b, a);
	}
	gmp_printf("%Zd\n", b);
}


// gcc gmp_test.c -lgmp -pg