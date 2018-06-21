SUMMARY = "Basic System Utilities"

inherit packagegroup

PACKAGES = "seco-base-utils"

RDEPENDS_seco-base-utils = "\
    bridge-utils \
    curl \
    dhcp-client \
    dnsmasq \
    lsof \
    minicom \
    modem-utils \
    ntpdate \
    packagegroup-core-ssh-openssh \
    setserial \
    usbutils \
    vim \
    which \
    wget \
    "

