include seco-synthetic-brain.inc

LIC_FILES_CHKSUM = "file://LICENSE;md5=2c1c00f9d3ed9e24fa69b932b7e7aff2"

SRC_URI[md5sum] = "5d34079b042adaf5758dc08d49680d12"
SRC_URI[sha256sum] = "5c46851e8b5f61d24ad080430f4e15fe14f3cf6f811949c2137570d6346b7bdc"

RDEPENDS_${PN} += "nodejs (>= 6.0.0)"

PR = "r0"
