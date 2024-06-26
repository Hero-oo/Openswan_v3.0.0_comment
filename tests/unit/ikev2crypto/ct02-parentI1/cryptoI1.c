#define SEAM_RSASIG
#include "../lp02-parentI1/parentI1_head.c"
#include "seam_demux.c"
#include "seam_pending.c"
#include "seam_whack.c"
#include "seam_initiate.c"
#include "seam_dnskey.c"
#include "seam_x509.c"
#include "seam_dh_v2.c"
#include "seam_ke.c"
#include "seam_natt.c"
#include "seam_host_parker.c"
#include "seam_gi_sha256_group14.c"
#include "seam_ikev2_sendI1.c"
#include "seam_finish.c"
#include "seam_rsa_check.c"

#define TESTNAME "cryptoI1"

static void init_local_interface(void)
{
	nat_traversal_support_non_ike = TRUE;
	nat_traversal_support_port_floating = TRUE;
	nat_traversal_enabled = TRUE;
	init_parker_interface(TRUE);
}

static void init_fake_secrets(void)
{
	osw_load_preshared_secrets(&pluto_secrets, TRUE,
				   SAMPLEDIR "parker.secrets", NULL, NULL);
}

#define INIT_LOADED init_loaded
static struct connection *init_loaded(struct connection *c)
{
	init_crypto();
	return c;
}

#include "../lp02-parentI1/parentI1_main.c"

/*
 * Local Variables:
 * c-style: pluto
 * c-basic-offset: 4
 * compile-command: "make check"
 * End:
 */
