#!/bin/sh

echo "extend rootfs to max size"
resize2fs /dev/mmcblk0p16

echo "libreader start!!!"
/usr/bin/libreader 1080P_50
sleep 2
