export TOOLCHAIN=/opt/fsl-imx-xwayland/4.9.51-mx8-beta/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux
${TOOLCHAIN}/aarch64-poky-linux-gcc -I /opt/fsl-imx-xwayland/4.9.51-mx8-beta/sysroots/aarch64-poky-linux/usr/include -L /opt/fsl-imx-xwayland/4.9.51-mx8-beta/sysroots/aarch64-poky-linux/usr/lib event.c -Wall

