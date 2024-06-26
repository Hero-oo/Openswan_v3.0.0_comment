/*
 * IKE modular algorithm handling interface
 * Author: JuanJo Ciarlante <jjo-ipsec@mendoza.gov.ar>
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
 * Fixes by:
 * 	ML:	Mathieu Lafon <mlafon@arkoon.net>
 *
 * Fixes:
 * 	ML:	ike_alg_ok_final() funcion (make F_STRICT consider hash/auth and modp).
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <openswan.h>
#include <openswan/ipsec_policy.h>

#include "sysdep.h"
#include "constants.h"
#include "pluto/defs.h"
#include "sha1.h"
#include "md5.h"
#include "pluto/crypto.h"

#include "packet.h"
#include "oswlog.h"
#include "whack.h"
#include "alg_info.h"
#include "pluto/ike_alg.h"
#include "pluto/db_ops.h"
#include "id.h"
#include "pluto/connections.h"
#include "pluto/kernel.h"
#include "pluto/plutoalg.h"

/* XXX many do not like this macro */
#define return_on(var, val)      \
	{                        \
		(var) = (val);   \
		goto return_out; \
	}

/*==========================================================
 *
 * 	IKE algo list handling
 *
 * 	- registration
 * 	- lookup
 *=========================================================*/
struct ike_alg *ike_alg_base[IKEv2_TRANS_TYPE_COUNT + 1] = { NULL, NULL };

#ifdef IKEV1
/*	check if IKE encrypt algo is present */
bool ikev1_alg_enc_present(int ealg, unsigned keysize)
{
	struct ike_encr_desc *encr_desc = ike_alg_get_encr(ealg);

	if (encr_desc == NULL)
		return FALSE;

	if (keysize != 0 && (keysize < encr_desc->keyminlen ||
			     keysize > encr_desc->keymaxlen)) {
		return FALSE;
	}

	return TRUE;
}
#endif

/*	check if IKEv2 encrypt algo is present */
bool ike_alg_enc_present(int ealg, unsigned keysize)
{
	struct ike_encr_desc *encr_desc = ike_alg_get_encr(ealg);

	if (encr_desc == NULL)
		return FALSE;

	if (keysize != 0 && (keysize < encr_desc->keyminlen ||
			     keysize > encr_desc->keymaxlen)) {
		return FALSE;
	}

	return TRUE;
}

/*	check if IKE hash algo is present */
bool ikev1_alg_integ_present(int halg, unsigned int keysize)
{
	struct ike_integ_desc *integ_desc =
		(struct ike_integ_desc *)ike_alg_ikev1_find(
			IKEv2_TRANS_TYPE_INTEG, halg, keysize);
	return integ_desc ? integ_desc->hash_digest_len : 0;
}

/*	check if IKE hash algo is present */
bool ikev2_alg_integ_present(int halg, unsigned int keysize)
{
	struct ike_integ_desc *integ_desc = ike_alg_get_integ(halg);
	return integ_desc ? integ_desc->hash_digest_len : 0;
}

/*	check if IKE PRF algo is present */
bool ike_alg_prf_present(int prfalg)
{
	struct ike_prf_desc *prf_desc = ike_alg_get_prf(prfalg);
	if (prf_desc != NULL)
		return TRUE;
	return FALSE;
}

/*	check if IKE DH group algo is present */
bool ike_alg_group_present(int modpid)
{
	struct ike_dh_desc *modp_desc = ike_alg_get_dh(modpid);
	if (modp_desc)
		return TRUE;
	return FALSE;
}

// DEPRECATE?
bool ike_alg_enc_ok(int ealg, unsigned key_len,
		    struct alg_info_ike *alg_info_ike __attribute__((unused)),
		    const char **errp, char *ugh_buf, size_t ugh_buf_len)
{
	int ret = TRUE;
	struct ike_encr_desc *enc_desc;

	enc_desc = ike_alg_get_encr(ealg);
	if (!enc_desc) {
		/* failure: encrypt algo must be present */
		snprintf(ugh_buf, ugh_buf_len, "encrypt algo not found");
		ret = FALSE;
	} else if ((key_len) && ((key_len < enc_desc->keyminlen) ||
				 (key_len > enc_desc->keymaxlen))) {
		/* failure: if key_len specified, it must be in range */
		snprintf(ugh_buf, ugh_buf_len,
			 "key_len not in range: encalg=%d, "
			 "key_len=%d, keyminlen=%d, keymaxlen=%d",
			 ealg, key_len, enc_desc->keyminlen,
			 enc_desc->keymaxlen);
		plog("ike_alg_enc_ok(): %.*s", (int)ugh_buf_len, ugh_buf);
		ret = FALSE;
	}

	DBG(
		DBG_KLIPS,
		if (ret) {
			DBG_log("ike_alg_enc_ok(ealg=%d,key_len=%d): "
				"blocksize=%d, keyminlen=%d, "
				"keydeflen=%d, keymaxlen=%d, "
				"ret=%d",
				ealg, key_len, (int)enc_desc->enc_blocksize,
				enc_desc->keyminlen, enc_desc->keydeflen,
				enc_desc->keymaxlen, ret);
		} else {
			DBG_log("ike_alg_enc_ok(ealg=%d,key_len=%d): NO", ealg,
				key_len);
		});
	if (!ret && errp)
		*errp = ugh_buf;
	return ret;
}

bool ikev1_alg_enc_ok(int ealg, unsigned key_len,
		      struct alg_info_ike *alg_info_ike __attribute__((unused)),
		      const char **errp, char *ugh_buf, size_t ugh_buf_len)
{
	int ret = TRUE;
	struct ike_encr_desc *enc_desc;

	enc_desc = ikev1_alg_get_encr(ealg);
	if (!enc_desc) {
		/* failure: encrypt algo must be present */
		snprintf(ugh_buf, ugh_buf_len, "encrypt algo not found");
		ret = FALSE;
	} else if ((key_len) && ((key_len < enc_desc->keyminlen) ||
				 (key_len > enc_desc->keymaxlen))) {
		/* failure: if key_len specified, it must be in range */
		snprintf(ugh_buf, ugh_buf_len,
			 "key_len not in range: encalg=%d, "
			 "key_len=%d, keyminlen=%d, keymaxlen=%d",
			 ealg, key_len, enc_desc->keyminlen,
			 enc_desc->keymaxlen);
		plog("ike_alg_enc_ok(): %.*s", (int)ugh_buf_len, ugh_buf);
		ret = FALSE;
	}

	DBG(
		DBG_KLIPS,
		if (ret) {
			DBG_log("ike_alg_enc_ok(ealg=%d,key_len=%d): "
				"blocksize=%d, keyminlen=%d, "
				"keydeflen=%d, keymaxlen=%d, "
				"ret=%d",
				ealg, key_len, (int)enc_desc->enc_blocksize,
				enc_desc->keyminlen, enc_desc->keydeflen,
				enc_desc->keymaxlen, ret);
		} else {
			DBG_log("ike_alg_enc_ok(ealg=%d,key_len=%d): NO", ealg,
				key_len);
		});
	if (!ret && errp)
		*errp = ugh_buf;
	return ret;
}

/*
 * ML: make F_STRICT logic consider enc,hash/auth,modp algorithms
 */
bool ikev1_alg_ok_final(int ealg, unsigned key_len, int aalg,
			unsigned int group, struct alg_info_ike *alg_info_ike)
{
	/*
	 * simple test to toss low key_len, will accept it only
	 * if specified in "esp" string
	 */
	int ealg_insecure = (key_len < 128);

	if (ealg_insecure || (alg_info_ike && alg_info_ike->alg_info_flags &
						      ALG_INFO_F_STRICT)) {
		int i;
		struct ike_info *ike_info;
		if (alg_info_ike) {
			ALG_INFO_IKE_FOREACH(alg_info_ike, ike_info, i)
			{
				if ((ike_info->ike_ealg == ealg) &&
				    ((ike_info->ike_eklen == 0) ||
				     (key_len == 0) ||
				     (ike_info->ike_eklen == key_len)) &&
				    (ike_info->ike_halg == aalg) &&
				    (ike_info->ike_modp == group)) {
					if (ealg_insecure)
						loglog(RC_LOG_SERIOUS,
						       "You should NOT use insecure IKE algorithms (%s)!",
						       enum_name(
							       &oakley_enc_names,
							       ealg));
					return TRUE;
				}
			}
		}
		openswan_log(
			"IKE Transform [%s (%d), %s, %s] refused due to %s",
			enum_name(&oakley_enc_names, ealg), key_len,
			enum_name(&oakley_hash_names, aalg),
			enum_name(&oakley_group_names, group),
			ealg_insecure ?
				"insecure key_len and enc. alg. not listed in \"ike\" string" :
				"strict flag");
		return FALSE;
	}
	return TRUE;
}

/*
 * 	return ike_algo object by {type, id} (for IKEv1)
 */
struct ike_alg *ike_alg_ikev1_find(enum ikev2_trans_type algo_type,
				   unsigned algo_id, unsigned keysize UNUSED)
{
	struct ike_alg *e = ike_alg_base[algo_type];
	for (; e != NULL; e = e->algo_next) {
		if (e->ikev1_algo_id == algo_id)
			break;
	}
	return e;
}

/*
 * 	return ike_algo object by {type, id} (for IKEv2)
 */
struct ike_alg *ike_alg_ikev2_find(enum ikev2_trans_type algo_type,
				   enum ikev2_trans_type_encr algo_v2id,
				   unsigned keysize __attribute__((unused)))
{
	struct ike_alg *e = ike_alg_base[algo_type];
	for (; e != NULL; e = e->algo_next) {
		if (e->algo_v2id == algo_v2id)
			break;
	}
	return e;
}

/*
 * 	Main "raw" ike_alg list adding function
 */
int ike_alg_add(struct ike_alg *a, bool quiet)
{
	int ret = 0;
	const char *ugh = "No error";
	if (a->algo_type > IKEv2_TRANS_TYPE_COUNT && a->algo_type < 0) {
		ugh = "Invalid algo_type is larger then IKEv2_TRANS_TYPE_COUNT";
		return_on(ret, -EINVAL);
	}
	if (ike_alg_ikev2_find(a->algo_type, a->algo_v2id, 0)) {
		ugh = "Algorithm type already exists";
		return_on(ret, -EEXIST);
	}
	if (ret == 0) {
		a->algo_next = ike_alg_base[a->algo_type];
		ike_alg_base[a->algo_type] = a;
	}
return_out:
	if (ret && !quiet) {
		openswan_log(
			"ike_alg_add(): ERROR: algo_type '%d', ikev1_algo_id '%d', %s",
			a->algo_type, a->ikev1_algo_id, ugh);
	}
	return ret;
}

/*
 * 	Validate and register IKE hash algorithm object
 */
int ike_alg_register_integ(struct ike_integ_desc *hash_desc)
{
	const char *alg_name = "<none>";
	int ret = 0;

	if (hash_desc->common.ikev1_algo_id > IKEv2_AUTH_INVALID) {
		plog("ike_alg_register_hash(): hash alg=%d < max=%d",
		     hash_desc->common.ikev1_algo_id, OAKLEY_HASH_MAX);
		return_on(ret, -EINVAL);
	}
	if (hash_desc->hash_ctx_size > sizeof(union hash_ctx)) {
		plog("ike_alg_register_hash(): hash alg=%d has "
		     "ctx_size=%d > hash_ctx=%d",
		     hash_desc->common.ikev1_algo_id,
		     (int)hash_desc->hash_ctx_size,
		     (int)sizeof(union hash_ctx));
		return_on(ret, -EOVERFLOW);
	}
	if (!(hash_desc->hash_init && hash_desc->hash_update &&
	      hash_desc->hash_final)) {
		plog("ike_alg_register_hash(): hash alg=%d needs  "
		     "hash_init(), hash_update() and hash_final()",
		     hash_desc->common.ikev1_algo_id);
		return_on(ret, -EINVAL);
	}

	alg_name =
		enum_name(&oakley_hash_names, hash_desc->common.ikev1_algo_id);

	if (!alg_name) {
		plog("ike_alg_register_hash(): WARNING: hash alg=%d not found in "
		     "constants.c:oakley_hash_names  ",
		     hash_desc->common.ikev1_algo_id);
		alg_name = "<NULL>";
	}

	if (hash_desc->common.name == NULL) {
		hash_desc->common.name = clone_str(alg_name, "hasher name");
	}

return_out:
	if (ret == 0)
		ret = ike_alg_add((struct ike_alg *)hash_desc, TRUE);
	openswan_log("ike_alg_register_hash(): Activating %s: %s (ret=%d)",
		     alg_name, ret == 0 ? "Ok" : "FAILED", ret);
	return ret;
}

/*
 * 	Validate and register IKE PRF algorithm object
 */
int ike_alg_register_prf(struct ike_prf_desc *prf_desc)
{
	const char *alg_name = "<none>";
	int ret = 0;

	if (prf_desc->common.ikev1_algo_id > IKEv2_AUTH_INVALID) {
		plog("ike_alg_register_hash(): hash alg=%d < max=%d",
		     prf_desc->common.ikev1_algo_id, OAKLEY_HASH_MAX);
		return_on(ret, -EINVAL);
	}
	if (prf_desc->hash_ctx_size > sizeof(union hash_ctx)) {
		plog("ike_alg_register_hash(): hash alg=%d has "
		     "ctx_size=%d > hash_ctx=%d",
		     prf_desc->common.ikev1_algo_id,
		     (int)prf_desc->hash_ctx_size, (int)sizeof(union hash_ctx));
		return_on(ret, -EOVERFLOW);
	}
	if (!(prf_desc->hash_init && prf_desc->hash_update &&
	      prf_desc->hash_final)) {
		plog("ike_alg_register_hash(): hash alg=%d needs  "
		     "hash_init(), hash_update() and hash_final()",
		     prf_desc->common.ikev1_algo_id);
		return_on(ret, -EINVAL);
	}

	alg_name = enum_name(&trans_type_prf_names, prf_desc->common.algo_v2id);

	if (!alg_name) {
		plog("ike_alg_register_hash(): WARNING: hash alg=%d not found in "
		     "constants.c:oakley_hash_names  ",
		     prf_desc->common.ikev1_algo_id);
		alg_name = "<NULL>";
	}

	if (prf_desc->common.name == NULL) {
		prf_desc->common.name = clone_str(alg_name, "hasher name");
	}

return_out:
	if (ret == 0)
		ret = ike_alg_add((struct ike_alg *)prf_desc, TRUE);
	openswan_log("ike_alg_register_prf(): Activating %s: %s (ret=%d)",
		     alg_name, ret == 0 ? "Ok" : "FAILED", ret);
	return ret;
}

/*
 * 	Validate and register IKE encryption algorithm object
 */
int ike_alg_register_enc(struct ike_encr_desc *enc_desc)
{
	const char *alg_name;
	int ret = 0;

	if (enc_desc->common.ikev1_algo_id > IKEv2_ENCR_INVALID) {
		plog("ike_alg_register_enc(): enc alg=%d < max=%d\n",
		     enc_desc->common.ikev1_algo_id, IKEv2_ENCR_INVALID);
		return_on(ret, -EINVAL);
	}

	/* XXX struct algo_aes_ccm_8 up to algo_aes_gcm_16, where
	 * "commin.ikev1_algo_id" is not defined need this officename fallback.
	 * These are defined in kernel_netlink.c and need to move to
	 * the proper place - even if klips does not support these
	 */
	alg_name = enum_name(&oakley_enc_names, enc_desc->common.ikev1_algo_id);
	if (!alg_name) {
		alg_name = enc_desc->common.officname;
		if (!alg_name) {
			plog("ike_alg_register_enc(): WARNING: enc alg=%d not found in "
			     "constants.c:oakley_enc_names  ",
			     enc_desc->common.ikev1_algo_id);
			alg_name = "<NULL>";
		}
	}

return_out:
	if (ret == 0) {
		ret = ike_alg_add((struct ike_alg *)enc_desc, FALSE);
	}
	openswan_log("ike_alg_register_enc(): Activating %s: %s (ret=%d)",
		     alg_name, ret == 0 ? "Ok" : "FAILED", ret);
	return 0;
}

/*
 * Local Variables:
 * c-basic-offset:4
 * c-style: pluto
 * End:
 */
