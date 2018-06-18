SUMMARY = "Basic System Utilities"

inherit packagegroup

PACKAGES = "seco-base-utils"

RDEPENDS_seco-base-utils = "\
    curl \
    dhcp-client \
    ifupdown \
    minicom \
    ntpdate \
    packagegroup-core-ssh-openssh \
    ppp \
    setserial \
    vim \
    wget \
    "

