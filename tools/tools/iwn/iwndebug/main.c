/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2021 Sofian Brabez <sbz@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification, immediately at the beginning of the file.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#include <sys/types.h>
#include <sys/sysctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sysexits.h>

#include <net80211/ieee80211.h>

#include "if_iwnreg.h"
#include "if_iwn_debug.h"

#ifndef IWN_DEBUG_NONE
#define IWN_DEBUG_NONE	0x00000000
#endif

static struct {
	const char 	*level;
	int		value;
} iwn_levels[] = {
	{"none",	IWN_DEBUG_NONE		},
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

#ifndef nitems
#define nitems(array)	(sizeof((array)) / sizeof((array)[0]))
#endif

static void
iwn_set_level(const char *oid, int level) {

	if (sysctlbyname(oid, NULL, NULL, &level, sizeof(level)) == -1)
		errx(1, "sysctlbyname %s:%d", __func__, __LINE__);
}

static char *
iwn_get_interface(const char *oid) {

	char *device;
	size_t len;

	if (sysctlbyname(oid, NULL, &len, NULL, 0) == -1)
		errx(1, "sysctlbyname %s:%d", __func__, __LINE__);

	device = malloc(len);
	device[len] = '\0';

	if (sysctlbyname(oid, device, &len, NULL, 0) == -1)
		errx(1, "sysctlbyname %s:%d", __func__, __LINE__);


	return device;
}

static void
iwn_print_levels(FILE *stream) {
	size_t i;

	fprintf(stream, "Possible debug levels:\n");

	for (i=0; i< nitems(iwn_levels); i++) {
		fprintf(stream, "\t%s\n", iwn_levels[i].level);
	}
}

int
main(int argc, char *argv[]) {
	int value;
	int found;
	size_t i;

	char *oid;
	char *ifname;

	found = 0;

	if (argc == 1) {
		fprintf(stderr, "Usage: %s <level>\n", getprogname());
		exit(EX_USAGE);
	}

	ifname = iwn_get_interface("net.wlan.devices");

	asprintf(&oid, "dev.iwn.%s.debug", ifname+3);

	if (!strncmp(argv[1], "-?", 2) || !strncmp(argv[1], "-h", 2)) {
		iwn_print_levels(stderr);
		exit(EX_USAGE);
	}

	for (i=0; i< nitems(iwn_levels); i++) {
		if (strcmp(argv[1], iwn_levels[i].level) == 0) {
			found = 1;
			value = iwn_levels[i].value;
		}
	}

	if (!found) {
		fprintf(stderr, "Invalid input\n");
		exit(EX_DATAERR);
	}

	iwn_set_level(oid, value);

	free(ifname);
	free(oid);

	exit(EX_OK);
}
