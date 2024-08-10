require recipes-mediacenter/kodi/stb-kodi_21.bb

SRC_URI:append = "file://hiplayer-21.patch"

DEPENDS += "uclan-libs-${MACHINE}"
PROVIDES += "virtual/kodi kodi"
RPROVIDES:${PN} += "virtual-kodi kodi"
RDEPENDS:${PN} += "uclan-libs-${MACHINE} uclan-opengl-${MACHINE}"
