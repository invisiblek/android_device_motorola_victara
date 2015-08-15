#!/sbin/sh

set -e

set -- $(cat /proc/cmdline)
for x in "$@"; do
    case "$x" in
        androidboot.carrier=*)
        carrier="${x#androidboot.carrier=}"
        ;;
    esac
done

case $carrier in
    "vzw")    variant="vzw" ;;
    *)        variant="gsm" ;;
esac

basedir="/system/blobs/$variant/"
cd $basedir
chmod 755 bin/*
find . -type f | while read file; do ln -s $basedir$file /system/$file ; done

exit 0
