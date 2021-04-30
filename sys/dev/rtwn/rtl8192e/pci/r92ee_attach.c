/*-
 * Copyright (c) 2021 Sofian Brabez
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include "opt_wlan.h"

#include <sys/param.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/mbuf.h>
#include <sys/kernel.h>
#include <sys/socket.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/queue.h>
#include <sys/taskqueue.h>
#include <sys/bus.h>
#include <sys/endian.h>
#include <sys/linker.h>

/* DMA bus defintion */
#include <machine/bus.h>
#include <machine/resource.h>
#include <sys/rman.h>

/* Network */
#include <net/if.h>
#include <net/ethernet.h>
#include <net/if_media.h>

/* Wireless */
#include <net80211/ieee80211_var.h>
#include <net80211/ieee80211_radiotap.h>

/* Device general */
#include <dev/rtwn/if_rtwnreg.h>
#include <dev/rtwn/if_rtwnvar.h>
#include <dev/rtwn/if_rtwn_nop.h>

/* Device PCI */
#include <dev/rtwn/pci/rtwn_pci_var.h>

/* Device Model */
#include <dev/rtwn/rtl8192e/r92e_priv.h>
#include <dev/rtwn/rtl8192e/r92e_reg.h>
#include <dev/rtwn/rtl8192e/r92e_var.h>

/* Device PCI Model specific */
#include <dev/rtwn/rtl8192e/pci/r92ee.h>


void	r92ee_attach(struct rtwn_pci_softc *);

static void
r92ee_attach_private(struct rtwn_softc *sc)
{
    /* TODO */
}

void
r92ee_attach(struct rtwn_pci_softc *pc)
{
    struct rtwn_softc *sc   = &pc->pc_sc;

    /* TODO: 
     *
     * Set pci softc structure fields
     *
     * pc->... = */

    r92ee_attach_private(sc);
}
