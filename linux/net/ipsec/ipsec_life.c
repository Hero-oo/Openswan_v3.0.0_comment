/*
 * @(#) lifetime structure utilities
 *
 * Copyright (C) 2001  Richard Guy Briggs  <rgb@freeswan.org>
 *                 and Michael Richardson  <mcr@freeswan.org>
 * Copyright (C) 2005-2006 Michael Richardson <mcr@xelerance.com>
 * Copyright (C) 2006 Paul Wouters <paul@xelerance.com>
 * Copyright (C) 2006 Bart Trojanowski <bart@jukie.net>
 * Copyright (C) 2012 David McCullough <david_mccullough@mcafee.com>
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

/*
 * This provides series of utility functions for dealing with lifetime
 * structures.
 *
 * ipsec_check_lifetime - returns -1    hard lifetime exceeded
 *                                 0    soft lifetime exceeded
 *                                 1    everything is okay
 *                        based upon whether or not the count exceeds hard/soft
 *
 */

#define __NO_VERSION__
#include <linux/module.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 38) && !defined(AUTOCONF_INCLUDED)
#include <linux/config.h>
#endif /* for CONFIG_IP_FORWARD */
#include <linux/kernel.h> /* printk() */

#include "openswan/ipsec_param.h"

#include <linux/netdevice.h> /* struct device, struct net_device_stats and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <openswan.h>

#include "openswan/radij.h"
#include "openswan/ipsec_life.h"
#include "openswan/ipsec_xform.h"
#include "openswan/ipsec_eroute.h"
#include "openswan/ipsec_encap.h"
#include "openswan/ipsec_radij.h"

#include "openswan/ipsec_sa.h"
#include "openswan/ipsec_tunnel.h"
#include "openswan/ipsec_ipe4.h"
#include "openswan/ipsec_ah.h"
#include "openswan/ipsec_esp.h"

#ifdef CONFIG_KLIPS_IPCOMP
#include "openswan/ipcomp.h"
#endif /* CONFIG_KLIPS_IPCOMP */

#include <openswan/pfkeyv2.h>
#include <openswan/pfkey.h>

#include "openswan/ipsec_proto.h"

enum ipsec_life_alive
ipsec_lifetime_check(struct ipsec_lifetime64 *il64, const char *lifename,
		     const char *saname, enum ipsec_life_type ilt,
		     enum ipsec_direction idir, struct ipsec_sa *ips)
{
	__u64 count;
	const char *dir;

	if (saname == NULL) {
		saname = "unknown-SA";
	}

	if (idir == ipsec_incoming) {
		dir = "incoming";
	} else {
		dir = "outgoing";
	}

	if (ilt == ipsec_life_timebased) {
		count = ipsec_jiffieshz_elapsed(jiffies / HZ, il64->ipl_count);
	} else {
		count = il64->ipl_count;
	}

	if (il64->ipl_hard && (count > il64->ipl_hard)) {
		KLIPS_PRINT(
			debug_tunnel & DB_TN_XMIT,
			"klips_debug:ipsec_lifetime_check: "
			"hard %s lifetime of SA:<%s%s%s> %s has been reached, SA expired, "
			"%s packet dropped.\n",
			lifename, IPS_XFORM_NAME(ips), saname, dir);

		pfkey_expire(ips, 1);
		return ipsec_life_harddied;
	}

	if (il64->ipl_soft && (count > il64->ipl_soft)) {
		KLIPS_PRINT(
			debug_tunnel & DB_TN_XMIT,
			"klips_debug:ipsec_lifetime_check: "
			"soft %s lifetime of SA:<%s%s%s> %s has been reached, SA expiring, "
			"soft expire message sent up, %s packet still processed.\n",
			lifename, IPS_XFORM_NAME(ips), saname, dir);

		if (ips->ips_state != K_SADB_SASTATE_DYING) {
			pfkey_expire(ips, 0);
		}
		ips->ips_state = K_SADB_SASTATE_DYING;

		return ipsec_life_softdied;
	}
	return ipsec_life_okay;
}

void ipsec_lifetime_update_hard(struct ipsec_lifetime64 *lifetime,
				__u64 newvalue)
{
	if (newvalue &&
	    (!lifetime->ipl_hard || (newvalue < lifetime->ipl_hard))) {
		lifetime->ipl_hard = newvalue;

		if (!lifetime->ipl_soft &&
		    (lifetime->ipl_hard < lifetime->ipl_soft)) {
			lifetime->ipl_soft = lifetime->ipl_hard;
		}
	}
}

void ipsec_lifetime_update_soft(struct ipsec_lifetime64 *lifetime,
				__u64 newvalue)
{
	if (newvalue &&
	    (!lifetime->ipl_soft || (newvalue < lifetime->ipl_soft))) {
		lifetime->ipl_soft = newvalue;

		if (lifetime->ipl_hard &&
		    (lifetime->ipl_hard < lifetime->ipl_soft)) {
			lifetime->ipl_soft = lifetime->ipl_hard;
		}
	}
}

/*
 *
 * Local variables:
 * c-file-style: "linux"
 * End:
 *
 */
