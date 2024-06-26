#ifndef _MPZFUNCS_H
#define _MPZFUNCS_H

/* some MP utilities */
#include <gmp.h>

extern void n_to_mpz(MP_INT *mp, const u_char *nbytes, size_t nlen);

extern chunk_t mpz_to_n(const MP_INT *mp, size_t bytes);

/* renamed */
#define mpz_to_n2 mpz_to_n_autosize
extern chunk_t mpz_to_n_autosize(const MP_INT *mp);

/* var := mod(base ** exp, mod), ensuring var is mpz_inited */
#define mpz_init_powm(flag, var, base, exp, mod)          \
	{                                                 \
		if (!(flag))                              \
			mpz_init(&(var));                 \
		(flag) = TRUE;                            \
		mpz_powm(&(var), &(base), &(exp), (mod)); \
	}

#endif /* _MPZFUNCS_H */
