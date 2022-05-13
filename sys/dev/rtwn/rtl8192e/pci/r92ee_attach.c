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

/* Common part */

#include <dev/rtwn/rtl8192e/usb/r92eu.h>
#include <dev/rtwn/rtl8192c/r92c.h>
#include <dev/rtwn/rtl8188e/r88e.h>
#include <dev/rtwn/rtl8821a/r21a_reg.h>
#include <dev/rtwn/rtl8812a/usb/r12au.h>
#include <dev/rtwn/rtl8812a/usb/r12au_tx_desc.h>
#include <dev/rtwn/rtl8821a/usb/r21au.h>

void    r92ee_attach(struct rtwn_pci_softc *);

static void
r92ee_attach_private(struct rtwn_softc *sc)
{
    /* TODO */
    struct r92e_softc *rs;

    rs = malloc(sizeof(struct r92e_softc), M_RTWN_PRIV, M_WAITOK | M_ZERO);

    sc->sc_priv = rs;
}

static void
r92ee_detach_private(struct rtwn_softc *sc)
{
    struct r92e_softc *rs = sc->sc_priv;

    free(rs, M_RTWN_PRIV);
}

static void
r92ee_adj_devcaps(struct rtwn_softc *sc)
{
    /* TODO */
}

void
r92ee_attach(struct rtwn_pci_softc *pc)
{
    struct rtwn_softc *sc   = &pc->pc_sc;


    device_printf(sc->sc_dev, "%s -> enter\n", __func__);
    device_printf(sc->sc_dev, "attaching current device %p\n", sc->sc_dev);

    /* TODO:
     *
     * Set pci softc structure fields
     *
     * cf. struct rtwn_softc sys/dev/rtwn/if_rtwnvar.h
     *
     * pc->... = */

    /* Common part. */
    sc->sc_flags                = RTWN_FLAG_EXT_HDR;
    sc->sc_set_chan             = r92e_set_chan;
    sc->sc_fill_tx_desc         = r12a_fill_tx_desc;
    sc->sc_fill_tx_desc_raw     = r12a_fill_tx_desc_raw;
    sc->sc_fill_tx_desc_null    = r12a_fill_tx_desc_null;
    sc->sc_dump_tx_desc         = NULL;
    sc->sc_tx_radiotap_flags    = r12a_tx_radiotap_flags;
    sc->sc_rx_radiotap_flags    = r12a_rx_radiotap_flags;
    sc->sc_get_rx_stats         = r12a_get_rx_stats;
    sc->sc_get_rssi_cck         = r92e_get_rssi_cck;
    sc->sc_get_rssi_ofdm        = r88e_get_rssi_ofdm;
    sc->sc_classify_intr        = NULL;
    sc->sc_handle_tx_report     = r12a_ratectl_tx_complete;
    sc->sc_handle_c2h_report    = r92e_handle_c2h_report;
    sc->sc_check_frame          = rtwn_nop_int_softc_mbuf;
    sc->sc_rf_read              = r92e_rf_read;
    sc->sc_rf_write             = r92e_rf_write;
    sc->sc_check_condition      = r92c_check_condition;
    sc->sc_efuse_postread       = rtwn_nop_softc;
    sc->sc_parse_rom            = r92e_parse_rom;
    sc->sc_set_led              = r92e_set_led;
    sc->sc_power_on             = r92e_power_on;
    sc->sc_power_off            = r92e_power_off;
#ifndef RTWN_WITHOUT_UCODE
    sc->sc_fw_reset             = r92e_fw_reset;
    sc->sc_fw_download_enable   = r12a_fw_download_enable;
#endif
    sc->sc_llt_init             = r92e_llt_init;
    sc->sc_set_page_size        = rtwn_nop_int_softc;
    sc->sc_lc_calib             = r92c_lc_calib;
    sc->sc_iq_calib             = rtwn_nop_softc;
    sc->sc_read_chipid_vendor   = r92c_read_chipid_vendor;
    sc->sc_adj_devcaps          = r92ee_adj_devcaps;
    sc->sc_vap_preattach        = rtwn_nop_softc_vap;
    sc->sc_postattach           = rtwn_nop_softc;
    sc->sc_detach_private       = r92ee_detach_private;
#ifndef RTWN_WITHOUT_UCODE
    sc->sc_set_media_status     = r92e_set_media_status;
    sc->sc_set_rsvd_page        = r88e_set_rsvd_page;
    sc->sc_set_pwrmode          = r92e_set_pwrmode;
    sc->sc_set_rssi             = rtwn_nop_softc;
#else
    sc->sc_set_media_status     = rtwn_nop_softc_int;
#endif
    sc->sc_beacon_init          = r12a_beacon_init;
    sc->sc_beacon_enable        = r92c_beacon_enable;
    sc->sc_beacon_set_rate      = rtwn_nop_void_int;
    sc->sc_beacon_select        = r21a_beacon_select;
    sc->sc_temp_measure         = r88e_temp_measure;
    sc->sc_temp_read            = r88e_temp_read;
    sc->sc_init_tx_agg          = NULL;
    sc->sc_init_rx_agg          = NULL;
    sc->sc_init_ampdu           = rtwn_nop_softc;
    sc->sc_init_intr            = r12a_init_intr;
    sc->sc_init_edca            = r92c_init_edca;
    sc->sc_init_bb              = r92e_init_bb;
    sc->sc_init_rf              = r92e_init_rf;
    sc->sc_init_antsel          = rtwn_nop_softc;
    sc->sc_post_init            = NULL;
    sc->sc_init_bcnq1_boundary  = rtwn_nop_int_softc;

    sc->mac_prog                = &rtl8192eu_mac[0];
    sc->mac_size                = nitems(rtl8192eu_mac);
    sc->bb_prog                 = &rtl8192eu_bb[0];
    sc->bb_size                 = nitems(rtl8192eu_bb);
    sc->agc_prog                = &rtl8192eu_agc[0];
    sc->agc_size                = nitems(rtl8192eu_agc);
    sc->rf_prog                 = &rtl8192eu_rf[0];

    sc->name                    = "RTL8192EE";
    sc->fwname                  = "rtwn-rtl8192eefw";
    sc->fwsig                   = 0x92e;

    sc->page_count              = R92E_TX_PAGE_COUNT;
    sc->pktbuf_count            = 0;
    sc->ackto                   = 0x40;
    sc->npubqpages              = R92E_PUBQ_NPAGES;
    sc->page_size               = R92E_TX_PAGE_SIZE;
    sc->txdesc_len              = sizeof(struct r12au_tx_desc);
    sc->efuse_maxlen            = R92E_EFUSE_MAX_LEN;
    sc->efuse_maplen            = R92E_EFUSE_MAP_LEN;
    sc->rx_dma_size             = R92E_RX_DMA_BUFFER_SIZE;

    sc->macid_limit             = R12A_MACID_MAX + 1;
    sc->cam_entry_limit         = R12A_CAM_ENTRY_COUNT;
    sc->fwsize_limit            = R92E_MAX_FW_SIZE;
    sc->temp_delta              = R88E_CALIB_THRESHOLD;

    sc->bcn_status_reg[0]       = R92C_TDECTRL;
    sc->bcn_status_reg[1]       = R21A_DWBCN1_CTRL;
    sc->rcr                     = 0;

    sc->ntxchains               = 2;
    sc->nrxchains               = 2;

    r92ee_attach_private(sc);
}
