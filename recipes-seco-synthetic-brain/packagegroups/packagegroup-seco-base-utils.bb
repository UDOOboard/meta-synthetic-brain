SUMMARY = "Basic System Utilities"

inherit packagegroup

PACKAGES = "seco-base-utils"

RDEPENDS_seco-base-utils = "\
    curl \
    dhcp-client \
    minicom \
    wget \
    ntpdate \
    vim \
    "

