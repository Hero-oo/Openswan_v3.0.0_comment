#define INCLUDE_IKEV1_PROCESSING
#define INCLUDE_QUICK_MODE
#define OMIT_MAIN_MODE
#define NAPT_ENABLED 1
#define NAT_TRAVERSAL
#define SEAM_CRYPTO
#include "../lp12-parentR2/parentR2_head.c"
#include "seam_ikev1.c"
#include "seam_ikev1_aggr.c"
#include "seam_ikealg.c"
#include "seam_crypt.c"
#include "seam_gi_sha1.c"
#include "nat_traversal.h"
#include "seam_rsasig.c"
#include "seam_dpd.c"
#include "seam_ikev1_crypto.c"
#include "seam_ke.c"
#include "seam_dh_v2.c"
#include "seam_nonce.c"
#include "seam_gi_sha1.c"
#include "seam_finish.c"
#include "seam_ikev1_crypto.c"
#include "seam_mockxfrm.c"
#include "seam_host_moon.c"

#define TESTNAME "v1certQR1"

bool no_cr_send = TRUE;
long crl_check_interval = 0;

void delete_cryptographic_continuation(struct state *st)
{
}

static void init_local_interface(void)
{
	nat_traversal_support_non_ike = TRUE;
	nat_traversal_support_port_floating = TRUE;
	nat_traversal_enabled = TRUE;
	init_moon_interface(TRUE);
}

static void init_fake_secrets(void)
{
	prompt_pass_t pass;
	memset(&pass, 0, sizeof(pass));

	osw_init_ipsecdir(SAMPLEDIR "moon");
	osw_load_preshared_secrets(&pluto_secrets, TRUE,
				   SAMPLEDIR "moon.secrets", &pass, NULL);
}

static void init_loaded(void)
{
	oco = osw_init_options();

	/* loading X.509 CA certificates */
	load_authcerts("CA cert", oco->cacerts_dir, AUTH_CA);
	/* loading X.509 AA certificates */
	load_authcerts("AA cert", oco->aacerts_dir, AUTH_AA);
	/* loading X.509 OCSP certificates */
	load_authcerts("OCSP cert", oco->ocspcerts_dir, AUTH_OCSP);

	/* loading X.509 CRLs */
	load_crls();
	/* loading attribute certificates (experimental) */
	load_acerts();

	list_certs(1);

	list_authcerts("CA", AUTH_CA, 1);

	xfrm_init_base_algorithms();
}

#include "seam_gi_sha1.c"

static void update_ngi_tc3(struct pcr_kenonce *kn)
{
	if (kn->thespace.len == 0) {
		fprintf(stderr, "failed to setup crypto_req, exiting\n");
		exit(89);
	}
}

void recv_pcap_packet1ikev1(u_char *user, const struct pcap_pkthdr *h,
			    const u_char *bytes)
{
	struct state *st;
	struct pcr_kenonce *kn = &crypto_req->pcr_d.kn;

	recv_pcap_packet_gen(user, h, bytes);

	/* find st involved */
	st = state_with_serialno(1);
	if (st) {
		st->st_connection->extra_debugging =
			DBG_PRIVATE | DBG_CRYPT | DBG_PARSING | DBG_EMITTING |
			DBG_CONTROL | DBG_CONTROLMORE;
	}
}

void recv_pcap_packet2ikev1_128(u_char *user, const struct pcap_pkthdr *h,
				const u_char *bytes)
{
	struct state *st;
	struct pcr_kenonce *kn = &crypto_req->pcr_d.kn;

	recv_pcap_packet_gen(user, h, bytes);

	/* find st involved */
	st = state_with_serialno(1);
	if (st) {
		st->st_connection->extra_debugging =
			DBG_PRIVATE | DBG_CRYPT | DBG_PARSING | DBG_EMITTING |
			DBG_CONTROL | DBG_CONTROLMORE;
		update_ngi_tc3(kn);
		run_continuation(crypto_req);
	}

	passert(st->st_suspended_md == NULL);
}

void recv_pcap_packet3ikev1(u_char *user, const struct pcap_pkthdr *h,
			    const u_char *bytes)
{
	struct state *st;
	struct pcr_kenonce *kn = &crypto_req->pcr_d.kn;

	/* before receiving the packet, need to complete the async calculation of the g^xy */
	/* now fill in the KE values from a constant.. not calculated */
	clonetowirechunk(&kn->thespace, kn->space, &kn->secret, SS(secret.ptr),
			 SS(secret.len));
	clonetowirechunk(&kn->thespace, kn->space, &kn->n, SS(ni.ptr),
			 SS(ni.len));
	clonetowirechunk(&kn->thespace, kn->space, &kn->gi, SS(gi.ptr),
			 SS(gi.len));

	cur_debugging |= DBG_PRIVATE | DBG_CRYPT | DBG_PARSING | DBG_EMITTING |
			 DBG_CONTROL | DBG_CONTROLMORE;
	recv_pcap_packet_gen(user, h, bytes);

	/* find st involved */
	st = state_with_serialno(1);
	if (st) {
		run_continuation(crypto_req);
	}
}

#define PCAP_INPUT_COUNT 4
recv_pcap recv_inputs[PCAP_INPUT_COUNT] = {
	recv_pcap_packet1ikev1,
	recv_pcap_packet2ikev1_128,
	recv_pcap_packet3ikev1,
	recv_pcap_packet3ikev1,
};

#define FINISH_NEGOTIATION
static void finish_negotiation(void)
{
	volatile struct state *st;
	st = state_with_serialno(1);
	passert(st != NULL);

	passert(st->st_oakley.integ_hash == IKEv2_AUTH_HMAC_MD5_96);
	passert(st->st_oakley.prf_hash == IKEv2_AUTH_HMAC_MD5_96);
	passert(st->st_oakley.encrypt == IKEv2_ENCR_3DES);
	passert(st->st_oakley.enckeylen == 192);

	st = state_with_serialno(2);
	passert(st != NULL);

	passert(st->st_esp.present);
	passert(st->st_esp.attrs.transattrs.integ_hash ==
		IKEv2_AUTH_HMAC_MD5_96);
	passert(st->st_esp.attrs.transattrs.encrypt == IKEv2_ENCR_3DES);
	passert(st->st_esp.attrs.transattrs.enckeylen == 192);
}

#include "../lp12-parentR2/parentR2_main.c"

/*
 * Local Variables:
 * c-style: pluto
 * c-basic-offset: 4
 * compile-command: "make check"
 * End:
 */
