/* Loading of PEM encoded files with optional encryption
 * Copyright (C) 2001-2004 Andreas Steffen, Zuercher Hochschule Winterthur
 * Copyright (C) 2004-2008  Michael Richardson <mcr@xelerance.com>
 * Copyright (C) 2004-2009  Paul Wouters <paul@xelerance.com>
 * Copyright (C) 2009 Avesh Agarwal <avagarwa@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 */

#ifndef _PEM_H
#define _PEM_H

#include "certs.h"

extern err_t pemtobin(chunk_t *blob, prompt_pass_t *pass, const char *label,
		      bool *pgp);

#ifdef HAVE_LIBNSS
extern void do_3des_nss(u_int8_t *buf, size_t buf_len, u_int8_t *key,
			size_t key_size, u_int8_t *iv, bool enc);
#endif

#endif /* _PEM_H */
