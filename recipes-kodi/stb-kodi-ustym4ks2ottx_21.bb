require recipes-mediacenter/kodi/stb-kodi_${PV}.bb

SRC_URI:append = "file://hiplayer_21_opt.patch"

DEPENDS += "uclan-libs-${MACHINE}"
PROVIDES += "virtual/kodi kodi"
RPROVIDES:${PN} += "virtual-kodi kodi "
RDEPENDS:${PN} += "uclan-libs-${MACHINE} uclan-opengl-${MACHINE}"