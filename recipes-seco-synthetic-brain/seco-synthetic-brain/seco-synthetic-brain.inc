DESCRIPTION = "SECO Synthetic Brain Agent"
HOMEPAGE = "https://www.seco.com"
LICENSE = "Apache-2.0"

DEPENDS = "nodejs-native seco-iot-user"
RDEPENDS_${PN} += "bash python nodejs nodejs-npm seco-iot-user"

INSANE_SKIP_${PN} = "staticdev"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = "file://seco-synthetic-brain.service \
	file://seco-synthetic-brain-watcher.service \
	file://seco-synthetic-brain-watcher.path \
	file://seco-synthetic-brain-tmpfiles.conf \
	git://git@secogit.seco.com:3222/iot/agent.git;protocol=ssh;branch=develop_aws; \
	"

S = "${WORKDIR}/git"

IOT_USER = "seco-iot"
IOT_GROUP = "seco-iot"
IOT_USERDIR = "${localstatedir}/lib/${IOT_USER}"

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
NPM_INSTALL_FLAGS ?= "--build-from-source --production --no-optional --verbose"

DELETE_COMMUNITY_FILES_${PN} ?= "1"

do_compile() {
	export NPM_CONFIG_CACHE="${NPM_CACHE_DIR}"

	export LZZ_COMPAT=1

	# Clear cache
	npm cache clean

	# Remove optional dependencies
	sed '/function-queue/d' -i  ${S}/package.json
	sed '/openzwave-shared/d' -i  ${S}/package.json
	sed '/request/d' -i  ${S}/package.json
	sed '/serialport/d' -i  ${S}/package.json
	sed '/udoo-arduino-manager/d' -i  ${S}/package.json
	sed '/udoo-blu/d' -i  ${S}/package.json
	# Fix invalid JSON
	sed '$!N;s/",\n  }/"\n  }/;P;D' -i  ${S}/package.json

	# Install
	npm --registry=${NPM_REGISTRY} \
		--arch=${NPM_ARCH} \
		--target_arch=${NPM_ARCH} \
		${NPM_INSTALL_FLAGS} install

	# Clean
	npm dedupe --arch=${NPM_ARCH} --target_arch=${NPM_ARCH} 

	rm -rf ${S}/test
	rm -rf ${S}/debian

	rm -rf ${S}/node_modules/lzz-gyp/lzz-source
	rm -rf ${S}/node_modules/lzz-gyp/lzz-compiled
	rm -rf ${S}/node_modules/better-sqlite3/deps
	rm -rf ${S}/node_modules/better-sqlite3/build/Release/obj
	rm -rf ${S}/node_modules/better-sqlite3/build/Release/obj.target

	find ${S}/node_modules -type d \( -name 'test*' -o -name 'doc*' -o \
		-name 'example*' -o -name 'benchmark*' \) |
		xargs rm -rf

	find ${S}/node_modules -name "Makefile" -o -name ".travis.yml" -o \
		-name ".gitignore" -o -name "*.md" -o -name "*.h" -o \ 
		-name "*.c" -o -name "*.cpp" |
		xargs rm -f

	if [ "${DELETE_COMMUNITY_FILES_${PN}}" -eq "1" ]
	then
		rm -rf \
			src/mqtt/mqttGw.js \
			src/mqtt/mqttAwsUdooGw.js \
			src/mqtt/payload/udooMqttPayload.js \
			src/manager/AwsCertsManager.js \
			src/manager/LocalHttpManager.js \
			src/manager/ReqManager.js \
			src/provider/arduinoProvider.js \
			src/provider/bluProvider.js \
			src/provider/zWaveProvider.js \
			src/provider/subProvider/arduino/ \
			src/provider/subProvider/blu/
	fi

	npm --arch=${NPM_ARCH} --target_arch=${NPM_ARCH} prune --production

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
	install -m 0644 ${WORKDIR}/${PN}-watcher.service ${D}${systemd_unitdir}/system/
	install -m 0644 ${WORKDIR}/${PN}-watcher.path ${D}${systemd_unitdir}/system/

	install -d ${D}${sysconfdir}/tmpfiles.d
	install -m 0644 ${WORKDIR}/${PN}-tmpfiles.conf ${D}${sysconfdir}/tmpfiles.d/

	install -d -m 0755 ${D}${IOT_USERDIR}/agent
	install -d -m 0751 ${D}${IOT_USERDIR}/agent/db
	install -d -m 0751 ${D}${IOT_USERDIR}/agent/certs

	chown -R ${IOT_USER} ${D}${IOT_USERDIR}
	chgrp -R ${IOT_USER} ${D}${IOT_USERDIR}
}

inherit systemd

SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "${PN}.service ${PN}-watcher.path"

PACKAGES = "${PN}"

FILES_${PN} = "\
	${IOT_USERDIR}/agent \
	${CLIENT_INSTALL_DIR}${PN} \
	${bindir} \
	${systemd_unitdir} \
	${sysconfdir}/tmpfiles.d/${PN}-tmpfiles.conf \
"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

