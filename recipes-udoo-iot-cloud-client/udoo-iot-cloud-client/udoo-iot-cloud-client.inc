DESCRIPTION = "Seco-IoT-Client"
HOMEPAGE = "http://seco.com"
LICENSE = "Apache-2.0"

DEPENDS = "nodejs-native"
RDEPENDS_${PN} += "bash python nodejs-npm"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = "file://${PN}_${PV}.zip \
	file://udoo-iot-cloud-client.service \
	"
# https://github.com/${PN}/${PN}/releases/download/${PV}/${PN}-${PV}.zip

S = "${WORKDIR}/${PN}-${PV}"

def get_nodejs_arch(d):
    target_arch = d.getVar('TRANSLATED_TARGET_ARCH', True)

    if target_arch == "x86-64":
        target_arch = "x64"
    elif target_arch == "aarch64":
        target_arch = "arm64"
    elif target_arch == "powerpc":
        target_arch = "ppc"
    elif target_arch == "powerpc64":
        target_arch = "ppc64"
    elif (target_arch == "i486" or target_arch == "i586" or target_arch == "i686"):
        target_arch = "ia32"

    return target_arch

NODE_MODULES_DIR = "${prefix}/lib/node_modules/"
CLIENT_INSTALL_DIR = "${prefix}/share/"
NPM_CACHE_DIR ?= "${WORKDIR}/npm_cache"
NPM_REGISTRY ?= "http://registry.npmjs.org/"
NPM_ARCH = "${@get_nodejs_arch(d)}"
#NPM_INSTALL_FLAGS ?= "--build-from-source --production --without-ssl --insecure --no-optional --verbose"
NPM_INSTALL_FLAGS ?= "--build-from-source --production --no-optional --verbose"

do_compile() {
	export NPM_CONFIG_CACHE="${NPM_CACHE_DIR}"

	export LZZ_COMPAT=1

	# Clear cache
	npm cache clear

	# Remove zwave - ek5
	sed '/openzwave-shared/d' -i  ${S}/package.json

	# Install
	npm --registry=${NPM_REGISTRY} --arch=${NPM_ARCH} --target_arch=${NPM_ARCH} ${NPM_INSTALL_FLAGS} install

	rm -rf ${S}/node_modules/lzz-gyp/lzz-compiled

	npm rebuild

	npm prune --production
}

do_install() {
	install -d ${D}${CLIENT_INSTALL_DIR}${PN}
	cp -r ${S}/* ${D}${CLIENT_INSTALL_DIR}${PN}

	# Set permissions
	chmod 0755 ${D}${CLIENT_INSTALL_DIR}${PN}/index.js

	# Symlinks
	install -d ${D}${bindir}
	ln -s ${CLIENT_INSTALL_DIR}${PN}/index.js ${D}${bindir}/${PN}
	
	install -d ${D}${systemd_unitdir}/system/
	install -m 0644 ${WORKDIR}/${PN}.service ${D}${systemd_unitdir}/system/
}

inherit systemd

SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "${PN}.service"

PACKAGES = "${PN}"

FILES_${PN} = "\
	${CLIENT_INSTALL_DIR}${PN} \
	${bindir} \
	${systemd_unitdir} \
"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
