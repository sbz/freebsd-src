#!/bin/bash

sudo dmesg -c &>/dev/null
sudo kldunload if_rtwn_pci
echo kld unloaded
