DESCRIPTION = "SECO Modem Utils"
HOMEPAGE = "https://www.seco.com"
LICENSE = "CLOSED"

RDEPENDS_${PN} += "bash ppp ppp-tools"

SRC_URI = "git://git@secogit.seco.com:3222/iot/modem-utils.git;protocol=ssh;"
SRCREV = "master"
PR = "r2"

S = "${WORKDIR}/git"
INSTALL_DIR = "${prefix}/sbin"
PPP_CONF_DIR = "etc/ppp"

do_install() {
	install -d ${D}${INSTALL_DIR}
	cp ${S}/bin/modemctl ${D}${INSTALL_DIR}

	install -d ${D}${PPP_CONF_DIR}
	cp ${S}/conf/ppp-on-dialer ${D}${PPP_CONF_DIR}

	install -d ${D}${systemd_unitdir}/system/
	install -m 0644 ${S}/modem-*.service ${D}${systemd_unitdir}/system/
}


inherit systemd
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "modem-hwinit.service modem-connection.service"

FILES_${PN} = "\
	${INSTALL_DIR} \
	${PPP_CONF_DIR} \
	${systemd_unitdir} \
"
