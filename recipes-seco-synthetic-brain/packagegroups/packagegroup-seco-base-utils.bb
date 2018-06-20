SUMMARY = "Basic System Utilities"

inherit packagegroup

PACKAGES = "seco-base-utils"

RDEPENDS_seco-base-utils = "\
    curl \
    dhcp-client \
    dnsmasq \
    lsof \
    minicom \
    ntpdate \
    packagegroup-core-ssh-openssh \
    ppp \
    ppp-tools \
    setserial \
    usbutils \
    vim \
    which \
    wget \
    "

