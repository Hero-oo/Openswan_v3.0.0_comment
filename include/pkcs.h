/* Support of PKCS#1 and PKCS#7 data structures
 * Copyright (C) 2002-2004 Andreas Steffen, Zuercher Hochschule Winterthur
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
 */

#ifndef _PKCS_H
#define _PKCS_H

extern bool parse_pkcs1_private_key(chunk_t blob, rsa_privkey_t *key);
extern bool parse_pkcs7_cert(chunk_t blob, x509cert_t **cert);

#endif /* _PKCS_H */
