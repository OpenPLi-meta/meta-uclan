KERNEL_MODULE_AUTOLOAD += "xfs"

# We know about rtc
MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"

MACHINE_ESSENTIAL_EXTRA_RDEPENDS = "\
    uclan-dvb-modules-${MACHINE} \
    uclan-loadmodules-${MACHINE} \ 
    uclan-opengl-${MACHINE} \
    uclan-libs-${MACHINE} \
    uclan-hihalt-${MACHINE} \
    uclan-reader-${MACHINE} \
    uclan-libreader-${MACHINE} \
    ${@bb.utils.contains("MACHINE_FEATURES", "blindscan", "uclan-blindscan-${MACHINE}" , "", d)} \    
    uclan-partitions-${MACHINE} \
    uclan-buildimage \
    e2fsprogs-resize2fs \
    gptfdisk \
"

MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS = "\
	kernel-module-cdfs \
	${@bb.utils.contains('MACHINE_FEATURES', 'emmc', 'partitions-by-name' , '', d)} \
	firmware-carl9170 \
	firmware-htc7010 \
	firmware-htc9271 \
	firmware-mt7601u \
	firmware-rt2870 \
	firmware-rt73 \
	firmware-rtl8712u \
	firmware-rtl8188eu \
	firmware-rtl8192eu \
	firmware-rtl8192cu \
	firmware-rtl8xxxu \
	firmware-zd1211 \
	\
	rtl8192eu \
	\
	kernel-module-ath9k-htc \
	kernel-module-carl9170 \
	kernel-module-mt7601u \
	kernel-module-r8712u \
	kernel-module-r8723au \
	kernel-module-r8188eu \
	kernel-module-rt2500usb \
	kernel-module-rt2800usb \
	kernel-module-rt73usb \
	kernel-module-rtl8187 \
	kernel-module-rtl8192cu \
	kernel-module-rtl8xxxu \
	kernel-module-zd1211rw \
	"

MACHINE_FEATURES += " alsa usbhost wlan switchoff osdposition hdmicec"

IMAGE_CLASSES  += "uclan-emmc_image"
IMAGE_FSTYPES += "uclanemmc"
IMAGE_ROOTFS_SIZE ?= "524288"

TARGET_ARCH = "arm"
DEFAULTTUNE = "cortexa15hf-neon-vfpv4"

PREFERRED_VERSION_linux-uclan = "4.4.176"

PREFERRED_PROVIDER_virtual/kernel = "linux-uclan"
PREFERRED_PROVIDER_virtual/egl = "uclan-opengl-${MACHINE}" 
PREFERRED_PROVIDER_virtual/libgles1 = "uclan-opengl-${MACHINE}"
PREFERRED_PROVIDER_virtual/libgles2 = "uclan-opengl-${MACHINE}"
PREFERRED_PROVIDER_kodi = "kodi"
PREFERRED_PROVIDER_virtual/blindscan-dvbs = "uclan-blindscan-${MACHINE}"

PREFERRED_RPROVIDER_showiframe = "uclan-showiframe-${MACHINE}"
PREFERRED_RPROVIDER_aio-grab = "uclan-grab-${MACHINE}"
PREFERRED_PROVIDER_virtual/gstreamer1.0-dvbmediasink ?= "gstreamer1.0-plugin-dvbmediasink"

require conf/machine/include/tune-cortexa15.inc

# Qt
PACKAGECONFIG_GL_pn-qtbase = "gles2 eglfs linuxfb"
PACKAGECONFIG_FB_pn-qtbase = " "

MACHINE_EXTRA_RRECOMMENDS = " \
	ffmpeg \
	ntfs-3g \
	"

DVBMEDIASINK_CONFIG = "--with-h265 --with-vb8 --with-vb9 --with-spark --with-opus --with-wma --with-wmv --with-pcm --with-dts --with-eac3"

IMAGEDIR ?= "uclan/${MACHINE}"
IMGDEPLOYDIR ?= "${DEPLOY_DIR_IMAGE}"
IMAGEVERSION := "${DISTRO_NAME}-${DISTRO_VERSION}-${DATE}"
IMAGEVERSION[vardepsexclude] = "DATE"

IMAGE_CMD_uclanemmc_append = "\
    mkdir -p ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}; \
    mkdir -p ${DEPLOY_DIR_IMAGE}/userdata; \
    mkdir -p ${DEPLOY_DIR_IMAGE}/userdata/linuxrootfs1; \
    mkdir -p ${DEPLOY_DIR_IMAGE}/userdata/linuxrootfs2; \
    mkdir -p ${DEPLOY_DIR_IMAGE}/userdata/linuxrootfs3; \
    mkdir -p ${DEPLOY_DIR_IMAGE}/userdata/linuxrootfs4; \
    cp -fR ${IMAGE_ROOTFS}/* ${DEPLOY_DIR_IMAGE}/userdata/linuxrootfs1/; \
    dd if=/dev/zero of=${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/rootfs.ext4 seek=${IMAGE_ROOTFS_SIZE} count=60 bs=1024; \
    mkfs.ext4 -F -i 4096 ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/rootfs.ext4 -d ${DEPLOY_DIR_IMAGE}/userdata; \
    cd ${IMAGE_ROOTFS}; \
    tar -cvf ${DEPLOY_DIR_IMAGE}/rootfs.tar -C ${IMAGE_ROOTFS} .; \
    mv ${DEPLOY_DIR_IMAGE}/rootfs.tar ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/rootfs.tar; \
    bzip2 ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/rootfs.tar; \
    cp ${DEPLOY_DIR_IMAGE}/uImage ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/${KERNEL_FILE}; \
    cp ${DEPLOY_DIR_IMAGE}/uImage ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/${KERNEL_FILE}; \
    if [ -e ${DEPLOY_DIR_IMAGE}/logo-${DISTRO_NAME}.img ]; then \
        cp -f ${DEPLOY_DIR_IMAGE}/logo-${DISTRO_NAME}.img ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/logo.img; \
    fi; \
    echo ${IMAGEVERSION} > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/imageversion; \
    cd ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions; \
    mkupdate -s 00000003-00000001-01010101 -f ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/emmc_partitions.xml -d ${DEPLOY_DIR_IMAGE}/usb_update.bin; \ 
    cd ${DEPLOY_DIR_IMAGE}; \
    zip ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_multiboot_ofgwrite.zip ${IMAGEDIR}/*; \
    zip -j ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${DEPLOY_DIR_IMAGE}/usb_update.bin ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/bootargs.bin ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/fastboot.bin ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/apploader.bin; \
    if [ -e ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/6605s.upg ]; then \
        zip -j ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${DEPLOY_DIR_IMAGE}/${MACHINE}-partitions/6605s.upg; \
    fi; \
    rm -f ${DEPLOY_DIR_IMAGE}/*.ext4; \
    rm -f ${DEPLOY_DIR_IMAGE}/*.manifest; \
    rm -f ${DEPLOY_DIR_IMAGE}/.ext4; \
    rm -f ${DEPLOY_DIR_IMAGE}/.manifest; \
    rm -f -f ${DEPLOY_DIR_IMAGE}/*.json; \
    rm -Rf ${DEPLOY_DIR_IMAGE}/usb_update.bin; \
    rm -Rf ${IMAGEDIR}; \
    rm -Rf ${DEPLOY_DIR_IMAGE}/userdata; \
    "

IMAGE_CMD_prepend = " \
    rm -Rf ${IMAGE_ROOTFS}/tmp/*; \
    "
