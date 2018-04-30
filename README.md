meta-synthetic-brain
====================

Seco layer to add support for the Seco IoT Agent (SECO Synthetic Brain).

## Availiable Packages
* `seco-synthetic-brain`

Configuration
=============
1. Clone the `meta-synthetic-brain` layer to your project directory.
2. Add the `meta-synthetic-brain` layer to `conf/bblayers.conf`
```bitbake
	BBLAYERS += "path/to/meta-synthetic-brain"
```
3. Add dependency layers to `conf/bblayers.conf`
```bitbake
	BBLAYERS += "path/to/meta-openembedded/meta-oe"
	BBLAYERS += "path/to/meta-openembedded/meta-python"
	BBLAYERS += "path/to/meta-java"
```
Usage
=====

Add support for the agent into your image add the following to your distro config or `conf/auto.conf`

```bitbake
    CORE_IMAGE_EXTRA_INSTALL += "seco-synthetic-brain"
```

Credits
=======

Based on the `meta-iot-cloud` layer from [intel-iot-devkit][intel-iot], forked from [OpenLabs-WRLinux][wrlinux].

[intel-iot]: https://github.com/intel-iot-devkit/meta-iot-cloud
[wrlinux]: https://github.com/WindRiver-OpenSourceLabs/meta-iot-cloud


