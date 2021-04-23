/*-
 * Copyright (c) 2021 Sofian Brabez <sbz@FreeBSD.org>
 * All rights reserved.
 *
 */
#include <sys/types.h>
#include <sys/sysctl.h>

#include <net/if.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <err.h>

#include <getopt.h>

/*
#include <net80211/ieee80211_var.h>

#include <dev/iwn/if_iwnreg.h>
#include <dev/iwn/if_iwn_debug.h>*/

//FIXME: figure out how to not copy from if_iwn_debug.h
enum {
	IWN_DEBUG_XMIT		= 0x00000001,	/* basic xmit operation */
	IWN_DEBUG_RECV		= 0x00000002,	/* basic recv operation */
	IWN_DEBUG_STATE		= 0x00000004,	/* 802.11 state transitions */
	IWN_DEBUG_TXPOW		= 0x00000008,	/* tx power processing */
	IWN_DEBUG_RESET		= 0x00000010,	/* reset processing */
	IWN_DEBUG_OPS		= 0x00000020,	/* iwn_ops processing */
	IWN_DEBUG_BEACON 	= 0x00000040,	/* beacon handling */
	IWN_DEBUG_WATCHDOG 	= 0x00000080,	/* watchdog timeout */
	IWN_DEBUG_INTR		= 0x00000100,	/* ISR */
	IWN_DEBUG_CALIBRATE	= 0x00000200,	/* periodic calibration */
	IWN_DEBUG_NODE		= 0x00000400,	/* node management */
	IWN_DEBUG_LED		= 0x00000800,	/* led management */
	IWN_DEBUG_CMD		= 0x00001000,	/* cmd submission */
	IWN_DEBUG_TXRATE	= 0x00002000,	/* TX rate debugging */
	IWN_DEBUG_PWRSAVE	= 0x00004000,	/* Power save operations */
	IWN_DEBUG_SCAN		= 0x00008000,	/* Scan related operations */
	IWN_DEBUG_STATS		= 0x00010000,	/* Statistics updates */
	IWN_DEBUG_AMPDU		= 0x00020000,	/* A-MPDU specific Tx */
	IWN_DEBUG_REGISTER	= 0x20000000,	/* print chipset register */
	IWN_DEBUG_TRACE		= 0x40000000,	/* Print begin and start driver function */
	IWN_DEBUG_FATAL		= 0x80000000,	/* fatal errors */
	IWN_DEBUG_ANY		= 0xffffffff
};

static struct {
	const char 	*level;
	int		value;
} iwn_levels[] = {
	{"none",	0			},
	{"xmit", 	IWN_DEBUG_XMIT 		},
	{"recv", 	IWN_DEBUG_RECV 		},
	{"state", 	IWN_DEBUG_STATE 	},
	{"txpower", 	IWN_DEBUG_TXPOW 	},
	{"reset", 	IWN_DEBUG_RESET 	},
	{"ops", 	IWN_DEBUG_OPS 		},
	{"becon", 	IWN_DEBUG_BEACON 	},
	{"watchdog", 	IWN_DEBUG_WATCHDOG 	},
	{"intr", 	IWN_DEBUG_INTR 		},
	{"calibrate", 	IWN_DEBUG_CALIBRATE 	},
	{"node", 	IWN_DEBUG_NODE 		},
	{"led", 	IWN_DEBUG_LED 		},
	{"cmd", 	IWN_DEBUG_CMD 		},
	{"txrate", 	IWN_DEBUG_TXRATE 	},
	{"powersave", 	IWN_DEBUG_PWRSAVE 	},
	{"scan", 	IWN_DEBUG_SCAN 		},
	{"stats", 	IWN_DEBUG_STATS		},
	{"ampdu", 	IWN_DEBUG_AMPDU 	},
	{"register", 	IWN_DEBUG_REGISTER 	},
	{"trace", 	IWN_DEBUG_TRACE 	},
	{"fatal", 	IWN_DEBUG_FATAL 	},
	{"any", 	IWN_DEBUG_ANY 		},
};

#define nitems(array) (sizeof(array)/sizeof(array[0]))

static void
iwn_set_sysctl(const char *oid, int value) {

	if (sysctlbyname(oid, NULL, NULL, &value, sizeof(value)) < 0)
		errx(1, "sysctlbyname %s", __func__);
}

__unused static void
iwn_get_systcl(const char *oid, int *value) {

	size_t len;
	len = sizeof(value);

	if (sysctlbyname(oid, &value, &len, NULL, 0) < 0)
		errx(1, "sysctlbyname");
}

static void
iwn_print_levels(void) {
	int i;

	for (i=0; i<(int)nitems(iwn_levels); i++) {
		printf("%s\n", iwn_levels[i].level);
	}
}

int
main(int argc, char *argv[]) {

	int value=2, i;
	size_t len;
	char oid[256];

	if (argc == 1) {
		fprintf(stderr, "Usage: %s <level>\n", getprogname());
		exit(1);
	}

	snprintf(oid, sizeof(oid), "dev.iwn.0.debug");
	len = sizeof(oid);

	if (sysctlbyname(oid, &value, &len, NULL, 0) < 0)
		errx(1, "sysctlbyname");

	printf("Get oid %s=%d\n", oid, value);

	if (argv[1][0] == '?' || !strcmp(argv[1], "-h")) {
		iwn_print_levels();
		exit(1);
	}

	for (i=0; i<(int)nitems(iwn_levels); i++) {
		//printf("Argv: %s\n", argv[1]);
		//printf("Level: %s\n", iwn_levels[i].level);
		if (strcmp(argv[1], iwn_levels[i].level))
			continue;
		value = iwn_levels[i].value;
		printf("choose (%s, %d)\n", iwn_levels[i].level, iwn_levels[i].value);
	}

	iwn_set_sysctl(oid, value);

	printf("Set oid %s=%d\n", oid, value);

	exit(0);
}
