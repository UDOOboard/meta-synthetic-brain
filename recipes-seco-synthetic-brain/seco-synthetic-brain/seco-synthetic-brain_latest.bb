include seco-synthetic-brain.inc

LIC_FILES_CHKSUM = "file://LICENSE;md5=a3d2aceccb688f7f1e6293409419fa3c"

PV = "1.1.5+git${SRCPV}"
SRCREV = "${AUTOREV}"
RDEPENDS_${PN} += "nodejs (>= 6.0.0)"

PR = "r0"
