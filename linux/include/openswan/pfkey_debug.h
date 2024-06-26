/*
 * sanitize a string into a printable format.
 *
 * Copyright (C) 1998-2002  D. Hugh Redelmeier.
 * Copyright (C) 2003  Michael Richardson <mcr@freeswan.org>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/lgpl.txt>.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 */

#ifndef _FREESWAN_PFKEY_DEBUG_H
#define _FREESWAN_PFKEY_DEBUG_H

#ifdef __KERNEL__

/* note, kernel version ignores pfkey levels */
#define DEBUGGING(level, args...) KLIPS_PRINT(debug_pfkey, "klips_debug:" args)

#define ERROR(args...) printk(KERN_ERR "klips:" args)

#else

extern unsigned int pfkey_lib_debug;

extern int (*pfkey_debug_func)(const char *message, ...) PRINTF_LIKE(1);
extern int (*pfkey_error_func)(const char *message, ...) PRINTF_LIKE(1);

#define DEBUGGING(level, args...)                                     \
	if (pfkey_lib_debug & level) {                                \
		if (pfkey_debug_func != NULL) {                       \
			(*pfkey_debug_func)("pfkey_lib_debug:" args); \
		} else {                                              \
			printf("pfkey_lib_debug:" args);              \
		}                                                     \
	}

#define ERROR(args...)                                        \
	if (pfkey_error_func != NULL) {                       \
		(*pfkey_error_func)("pfkey_lib_debug:" args); \
	}

#define MALLOC(size) malloc(size)
#define FREE(obj) free(obj)

#endif

#endif
