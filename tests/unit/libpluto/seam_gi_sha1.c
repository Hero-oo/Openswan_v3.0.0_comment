#ifndef __seam_gi_sha1_c__
#define __seam_gi_sha1_c__
#include "seam_secrets.h"

/*
 * these files are created by running a full pluto, and observing the
 * debug lines, and transforming them to C data files
 */

/* test case 3 - DH operation, SHA1 + AES */
u_int16_t __tc3_oakleygroup = OAKLEY_GROUP_MODP1536;
oakley_auth_t __tc3_auth = AUTH_ALGORITHM_HMAC_SHA1;
enum oakley_hash_t __tc3_hash = OAKLEY_SHA1;
enum phase1_role __tc3_init = INITIATOR;

unsigned char __tc3_gi[] = {
	0xff, 0xbc, 0x6a, 0x92, 0xa6, 0xb9, 0x55, 0x9b, 0x05, 0xfa, 0x96, 0xa7,
	0xa4, 0x35, 0x07, 0xb4, 0xc1, 0xe1, 0xc0, 0x86, 0x1a, 0x58, 0x71, 0xd9,
	0xba, 0x73, 0xa1, 0x63, 0x11, 0x37, 0x88, 0xc0, 0xde, 0xbb, 0x39, 0x79,
	0xe7, 0xff, 0x0c, 0x52, 0xb4, 0xce, 0x60, 0x50, 0xeb, 0x05, 0x36, 0x9e,
	0xa4, 0x30, 0x0d, 0x2b, 0xff, 0x3b, 0x1b, 0x29, 0x9f, 0x3b, 0x80, 0x2c,
	0xcb, 0x13, 0x31, 0x8c, 0x2a, 0xb9, 0xe3, 0xb5, 0x62, 0x7c, 0xb4, 0xb3,
	0x5e, 0xb9, 0x39, 0x98, 0x20, 0x76, 0xb5, 0x7c, 0x05, 0x0d, 0x7b, 0x35,
	0xc3, 0xc5, 0xc7, 0xcc, 0x8c, 0x0f, 0xea, 0xb7, 0xb6, 0x4a, 0x7d, 0x7b,
	0x6b, 0x8f, 0x6b, 0x4d, 0xab, 0xf4, 0xac, 0x40, 0x6d, 0xd2, 0x01, 0x26,
	0xb9, 0x0a, 0x98, 0xac, 0x76, 0x6e, 0xfa, 0x37, 0xa7, 0x89, 0x0c, 0x43,
	0x94, 0xff, 0x9a, 0x77, 0x61, 0x5b, 0x58, 0xf5, 0x2d, 0x65, 0x1b, 0xbf,
	0xa5, 0x8d, 0x2a, 0x54, 0x9a, 0xf8, 0xb0, 0x1a, 0xa4, 0xbc, 0xa3, 0xd7,
	0x62, 0x42, 0x66, 0x63, 0xb1, 0x55, 0xd4, 0xeb, 0xda, 0x9f, 0x60, 0xa6,
	0xa1, 0x35, 0x73, 0xe6, 0xa8, 0x88, 0x13, 0x5c, 0xdc, 0x67, 0x3d, 0xd4,
	0x83, 0x02, 0x99, 0x03, 0xf3, 0xa9, 0x0e, 0xca, 0x23, 0xe1, 0xec, 0x1e,
	0x27, 0x03, 0x31, 0xb2, 0xd0, 0x50, 0xf4, 0xf7, 0x58, 0xf4, 0x99, 0x27,
};
unsigned int __tc3_gi_len = sizeof(__tc3_gi);

unsigned char __tc3_gr[] = {
	0xcd, 0x30, 0xdf, 0x6e, 0xc0, 0x85, 0x44, 0x12, 0x53, 0x01, 0x80, 0xd8,
	0x7e, 0x1a, 0xfb, 0xb3, 0x26, 0x79, 0x3e, 0x99, 0x56, 0xc8, 0x6a, 0x96,
	0x25, 0x53, 0xc2, 0x77, 0xad, 0x5b, 0xab, 0x50, 0xf8, 0x32, 0x5a, 0xd8,
	0x64, 0x0b, 0x0e, 0xfe, 0xa5, 0x1d, 0x6c, 0x83, 0x1f, 0xa1, 0x7c, 0xfb,
	0x0f, 0x2e, 0x1a, 0xf4, 0xb1, 0x66, 0xa0, 0xfe, 0x30, 0x75, 0x12, 0xad,
	0x0f, 0x81, 0xab, 0xb8, 0xaa, 0xfb, 0x68, 0x48, 0xec, 0x10, 0xa4, 0x97,
	0x6c, 0x3d, 0xb1, 0x17, 0xec, 0xe1, 0xe6, 0x61, 0xdb, 0xbf, 0x48, 0x0c,
	0x28, 0x2e, 0x3f, 0x11, 0x07, 0xc1, 0x86, 0x42, 0x80, 0x1e, 0xe8, 0x3f,
	0x9e, 0x4a, 0xb9, 0xab, 0x63, 0x6f, 0x23, 0x7d, 0xaa, 0xf6, 0xa7, 0xaa,
	0xd8, 0x22, 0x99, 0x3e, 0xa4, 0x1e, 0xa3, 0x31, 0xee, 0x27, 0x82, 0x0b,
	0x93, 0xf5, 0x0b, 0x8f, 0x3f, 0x71, 0x05, 0x61, 0xc9, 0x25, 0x70, 0x26,
	0x97, 0xba, 0x6b, 0x1e, 0x95, 0x3c, 0x21, 0xfb, 0xc9, 0xa7, 0x7d, 0x2b,
	0x5f, 0x87, 0x3c, 0xfc, 0x50, 0x99, 0xe7, 0x7d, 0x48, 0x4c, 0xdd, 0x52,
	0x66, 0x4b, 0xcf, 0x0d, 0xbf, 0x00, 0xca, 0xfd, 0xae, 0x6d, 0xe7, 0x14,
	0x6d, 0x11, 0x35, 0xf6, 0x5d, 0x93, 0x5f, 0x60, 0xb9, 0x73, 0x0f, 0xe0,
	0x49, 0x2c, 0x2a, 0xf8, 0xc9, 0x04, 0xf6, 0x4c, 0x59, 0x16, 0x90, 0x9d,
};
unsigned int __tc3_gr_len = sizeof(__tc3_gr);

unsigned char __tc3_ni[] = {
	0xb5, 0xce, 0x84, 0x19, 0x09, 0x5c, 0x6e, 0x2b,
	0x6b, 0x62, 0xd3, 0x05, 0x53, 0x05, 0xb3, 0xc4,
};
unsigned int __tc3_ni_len = sizeof(__tc3_ni);

unsigned char __tc3_nr[] = {
	0x47, 0xe9, 0xf9, 0x25, 0x8c, 0xa2, 0x38, 0x58,
	0xf6, 0x75, 0xb1, 0x66, 0xb0, 0x2c, 0xc2, 0x92,
};
unsigned int __tc3_nr_len = sizeof(__tc3_nr);

unsigned char __tc3_icookie[] = {
	0x75, 0x46, 0xd3, 0xd6, 0xea, 0x09, 0xf7, 0xdf,
};
unsigned int __tc3_icookie_len = sizeof(__tc3_icookie);

unsigned char __tc3_rcookie[] = {
	0x61, 0xa6, 0x78, 0x6a, 0x41, 0xea, 0x48, 0x06,
};
unsigned int __tc3_rcookie_len = sizeof(__tc3_rcookie);

unsigned char __tc3_secret[] = {
	0x17, 0x9b, 0xb3, 0x22, 0xa6, 0xab, 0xcd, 0xef, 0x01, 0x4e, 0x41,
	0x03, 0xf2, 0xf6, 0x2e, 0x93, 0xfb, 0x07, 0xd4, 0x93, 0x84, 0x57,
	0xe4, 0x54, 0x1e, 0x64, 0x46, 0xa9, 0x34, 0x37, 0xc4, 0x9d,
};
unsigned int __tc3_secret_len = sizeof(__tc3_secret);

unsigned char __tc3_secretr[] = {
	0x17, 0x9b, 0xb3, 0x22, 0xa6, 0xab, 0xcd, 0xef, 0x01, 0x4e, 0x41,
	0x03, 0xf2, 0xf6, 0x2e, 0x93, 0xfb, 0x07, 0xd4, 0x93, 0x84, 0x57,
	0xe4, 0x54, 0x1e, 0x64, 0x46, 0xa9, 0x34, 0x37, 0xc4, 0x9d,
};
unsigned int __tc3_secretr_len = sizeof(__tc3_secretr);

unsigned char __tc3_results_shared[] = {
	0x60, 0x76, 0x62, 0x71, 0x0c, 0x76, 0xfe, 0x27, 0x4f, 0x6f, 0x7c, 0x9f,
	0x68, 0xcf, 0xcf, 0xe0, 0xa7, 0x56, 0x44, 0xcb, 0xa1, 0x92, 0x2a, 0xb9,
	0xc9, 0xc2, 0x16, 0x0e, 0xa7, 0x80, 0xd0, 0x71, 0x50, 0x06, 0x87, 0x10,
	0x85, 0xf0, 0x80, 0x30, 0x58, 0xc4, 0x2e, 0xc9, 0xd6, 0xae, 0x4f, 0x53,
	0x41, 0xb6, 0x2b, 0x7c, 0x66, 0x8d, 0x9f, 0xba, 0x20, 0x15, 0x5a, 0x7c,
	0x23, 0x60, 0xe6, 0x22, 0x03, 0x55, 0xa0, 0x80, 0x3b, 0xc9, 0x11, 0xa5,
	0x6f, 0xc7, 0x30, 0x0d, 0xd7, 0xf6, 0x6b, 0x01, 0xc4, 0x88, 0xab, 0x8f,
	0x79, 0x1a, 0xd2, 0xcc, 0x58, 0x8d, 0x28, 0xc0, 0xc0, 0x78, 0xfa, 0x61,
	0x21, 0xd5, 0x88, 0x1b, 0x3a, 0xe9, 0xb4, 0xa7, 0xd8, 0xc9, 0xa6, 0x58,
	0x60, 0x6e, 0x0a, 0x2a, 0x5e, 0x9c, 0x5f, 0x9e, 0xc2, 0xb5, 0x0b, 0x63,
	0xd7, 0xaf, 0xbb, 0x80, 0xf4, 0xa4, 0x83, 0x37, 0xce, 0xde, 0xf5, 0xaa,
	0x6f, 0xa1, 0xa0, 0xf3, 0xaf, 0xc3, 0x21, 0x47, 0x19, 0x2c, 0x7f, 0xba,
	0x7e, 0x27, 0x9f, 0x00, 0x28, 0x81, 0x21, 0xcb, 0x3d, 0xe7, 0xf1, 0xd9,
	0x76, 0xfd, 0xf1, 0x6c, 0x99, 0x28, 0xdb, 0x95, 0x92, 0x38, 0xf7, 0x83,
	0xa4, 0xfd, 0x38, 0x75, 0x7e, 0x55, 0x2c, 0xe9, 0x2d, 0xdb, 0xd6, 0xf5,
	0x0c, 0x1d, 0x35, 0xaf, 0x77, 0x34, 0xfc, 0x85, 0xcb, 0x30, 0xb8, 0x09,
};

/* NOTE: some calculations below are known to be wrong */
unsigned char __tc3_results_skeyseed[] = {
	0x9d, 0x8e, 0x53, 0x96, 0x0b, 0x36, 0x01, 0xf4, 0xc0, 0xd3,
	0x29, 0xee, 0x1e, 0xc3, 0x70, 0x27, 0x0d, 0x06, 0x22, 0x17,
};

unsigned char __tc3_results_skey_d[] = {
	0xac, 0x97, 0xeb, 0x39, 0x07, 0x53, 0xa4, 0x5d, 0x61, 0xc1,
	0x64, 0xff, 0xca, 0x25, 0x99, 0xb0, 0x2e, 0x05, 0x3c, 0x7e,
};

unsigned char __tc3_results_skey_ai[] = {
	0x4e, 0xa8, 0xe6, 0x62, 0xb0, 0x7c, 0xdd, 0x43, 0x0f, 0x69,
	0x44, 0xc6, 0x72, 0x3e, 0x4b, 0x82, 0xd5, 0x72, 0x24, 0x18,
};

unsigned char __tc3_results_skey_ar[] = {
	0x51, 0x5b, 0x0b, 0xd2, 0x2e, 0x6d, 0x76, 0xb3, 0x4f, 0xdb,
	0x76, 0x0a, 0xa7, 0xbf, 0xad, 0x80, 0xb1, 0x09, 0xb7, 0x5d,
};

unsigned char __tc3_results_skey_ei[] = {
	0x3f, 0x44, 0xbf, 0x47, 0xca, 0xfd, 0x81, 0x50,
	0x59, 0x1d, 0xeb, 0x08, 0x81, 0x99, 0xfc, 0xbf,
};

unsigned char __tc3_results_skey_er[] = {
	0xbe, 0xdb, 0x67, 0xec, 0x7d, 0xc3, 0xd0, 0x0c,
	0xcc, 0xac, 0x42, 0xe7, 0x0c, 0xd6, 0x3b, 0xde,
};

unsigned char __tc3_results_skey_pi[] = {
	0xcc, 0x07, 0x97, 0x44, 0xb4, 0xa3, 0x4e, 0x8a, 0x0d, 0x2f,
	0x27, 0x8b, 0xee, 0x06, 0x6d, 0x07, 0xa5, 0xa5, 0x75, 0x2e,
};

unsigned char __tc3_results_skey_pr[] = {
	0xe9, 0x00, 0x11, 0x7e, 0x41, 0xd4, 0x31, 0x62, 0x40, 0xb8,
	0x63, 0x22, 0xbf, 0x06, 0x9f, 0xbc, 0xeb, 0x81, 0x58, 0xe7,
};

unsigned char __tc3_results_new_iv[] = { 0xbc, 0xd5, 0x89, 0x68, 0x0c, 0x42,
					 0x2e, 0xcd, 0x14, 0x72, 0xae, 0xa3,
					 0x97, 0x42, 0x67, 0x41 };

unsigned char __tc3_results_enc_key[] = {
	0x56, 0x4c, 0x62, 0xb3, 0x52, 0xd9, 0x33, 0x45, 0x0a, 0xd1, 0xdf, 0x32,
	0x4e, 0xa5, 0x7d, 0x69, 0x4e, 0xa5, 0x7d, 0x69, 0x0a, 0xd1, 0xdf, 0x32,
};

SEAM_SECRETS_DECLARE_USING_PREFIX_ARRAYS(tc3_secrets, OAKLEY_GROUP_MODP1536,
					 AUTH_ALGORITHM_HMAC_SHA1, OAKLEY_SHA1,
					 INITIATOR, IKEv2_PRF_HMAC_SHA1,
					 IKEv2_AUTH_HMAC_SHA1_96, __tc3);
#undef SECRETS
#define SECRETS (&tc3_secrets)

#define GI_SHA1_TC3
#endif /* TC3 */
