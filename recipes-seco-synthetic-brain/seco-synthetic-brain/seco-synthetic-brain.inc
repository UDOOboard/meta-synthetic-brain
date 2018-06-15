DESCRIPTION = "SECO Synthetic Brain Agent"
HOMEPAGE = "https://www.seco.com"
LICENSE = "Apache-2.0"

DEPENDS = "nodejs-native"
RDEPENDS_${PN} += "bash python nodejs nodejs-npm"

INSANE_SKIP_${PN} = "staticdev"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = "file://seco-synthetic-brain.service \
	git://git@secogit.seco.com:3222/iot/agent.git;protocol=ssh;branch=develop_aws; \
	"

#S = "${WORKDIR}/${OTHER_PN}-${PV}"
S = "${WORKDIR}/git"

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

# User Management
inherit useradd

IOT_USER = "seco-iot"
IOT_GROUP = "seco-iot"

USERADD_PACKAGES = "${PN}"
USERADD_PARAM_${PN}  = "-r -d ${localstatedir}/seco-sb \
			-s ${sbindir}/nologin \
			-U ${IOT_USER}"

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
	npm --registry=${NPM_REGISTRY} --arch=${NPM_ARCH} --target_arch=${NPM_ARCH} ${NPM_INSTALL_FLAGS} install

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

	install -d -o ${IOT_USER} -g ${IOT_GROUP} ${D}${localstatedir}/seco-sb/
	install -d -o ${IOT_USER} -g ${IOT_GROUP} ${D}${localstatedir}/seco-sb/db
	install -d -o ${IOT_USER} -g ${IOT_GROUP} ${D}${localstatedir}/seco-sb/certs
}

inherit systemd

SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "${PN}.service"

PACKAGES = "${PN}"

FILES_${PN} = "\
	${CLIENT_INSTALL_DIR}${PN} \
	${bindir} \
	${systemd_unitdir} \
	${localstatedir}/seco-sb \
"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

