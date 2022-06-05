#!/bin/bash -e

CLEAN=0

if [[ "$1" == '-h' ]]; then
    printf "Usage: %s <clean>\n" $0
    exit 64
fi

if [[ "$1" == "clean" ]]; then
    CLEAN=1
fi

cd ../rtwn/
[[ "$CLEAN" -eq 1 ]] && make clean
echo build rtwn
make
sudo cp /usr/obj/usr/src/amd64.amd64/sys/modules/rtwn/rtwn.ko /boot/kernel/
sudo kldxref /boot/kernel
cd -
[[ "$CLEAN" -eq 1 ]] && make clean
echo build rtwn_pci
make
echo clear dmesg
sudo dmesg -c
sudo kldload -v /usr/obj/usr/src/amd64.amd64/sys/modules/rtwn_pci/if_rtwn_pci.ko
echo loaded
dmesg

echo module loaded
kldstat | grep rtwn

echo pciconf
pciconf -vl pci0:3:0:0

echo sysctl debug
sudo sysctl dev.rtwn.0.debug=4294967295

#echo lscpi
#sudo lspci -vv -s 03:00.0 -x -nn
#sudo lspci -nn -v -xxxx -P -s 03:00.0
