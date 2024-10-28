SUMMARY = "grab for Uclan Model ${MACHINE}"
SECTION = "base"
PRIORITY = "optional"
LICENSE = "CLOSED"
PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "ustym4ks2ottx"

SRCDATE = "20221203"

PV = "${SRCDATE}"
PR = "r0"

RPROVIDES:${PN}  = "aio-grab"
RREPLACES:${PN}  = "aio-grab"
RCONFLICTS:${PN} = "aio-grab"

SRC_URI = "http://downloads.openpli.org/archive/uclan/${SOC_FAMILY}-grab-${SRCDATE}.tar.gz"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/grab ${D}/${bindir}
}

do_package_qa() {
}

FILES:${PN}  = "${bindir}/grab"

SRC_URI[md5sum] = "742ef5b1085b2d994dd9b22931c5b219"
SRC_URI[sha256sum] = "9395dd0a1eff99fabe3d362a2f26e1834f50b59d17f4ffb4392bbbcda97d73c9"
