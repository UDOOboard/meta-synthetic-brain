DESCRIPTION = "Creates seco-iot user"
HOMEPAGE = "https://www.seco.com"
PR = "r1"
LICENSE = "CLOSED"

S = "${WORKDIR}"

EXCLUDE_FROM_WORLD = "1"

inherit useradd

IOT_USER = "seco-iot"
IOT_GROUP = "seco-iot"
IOT_USERDIR = "${localstatedir}/lib/${IOT_USER}"

# You must set USERADD_PACKAGES when you inherit useradd. This
# lists which output packages will include the user/group
# creation code.
USERADD_PACKAGES = "${PN}"

# You must also set USERADD_PARAM and/or GROUPADD_PARAM when
# you inherit useradd.

# USERADD_PARAM specifies command line options to pass to the
# useradd command. Multiple users can be created by separating
# the commands with a semicolon. Here we'll create two users,
# user1 and user2:

USERADD_PARAM_${PN}  = "--system \
			--create-home \
			--home-dir ${IOT_USERDIR} \
			--groups tty \
			--shell ${sbindir}/nologin \
			--user-group ${IOT_USER} \
"

do_install () {
	install -d -m 755 ${D}${IOT_USERDIR}

	# The new users and groups are created before the do_install
	# step, so you are now free to make use of them:
	chown -R ${IOT_USER} ${D}${IOT_USERDIR}
	chgrp -R ${IOT_USER} ${D}${IOT_USERDIR}
}

FILES_${PN} = " ${IOT_USERDIR} \
	"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
