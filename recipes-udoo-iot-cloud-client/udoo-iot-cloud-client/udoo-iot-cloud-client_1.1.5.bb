include udoo-iot-cloud-client.inc

LIC_FILES_CHKSUM = "file://LICENSE;md5=062ce26333f4852fde7d71ca0cc7b03d"

SRC_URI[md5sum] = "0d0c07ab9ab1dc847a085792d3d93f1f"
SRC_URI[sha256sum] = "96b66fd4afdfebb7239323d4cad7e0c63c34466a9d1b3c4e4d90b7a37dcbefe7"

RDEPENDS_${PN} += "nodejs (>= 6.0.0)"

PR = "r0"
