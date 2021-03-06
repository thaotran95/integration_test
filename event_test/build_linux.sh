export TOOLCHAIN_aarch64=/opt/sdk/imx8dvsabreauto_wayland/x86_64-linux/usr/bin/aarch64-poky-linux
export TOOLCHAIN_armhf=/opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin
export TOOLCHAIN_wayland=/opt/sdk/poky/2.1.2/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux
${TOOLCHAIN_aarch64}/aarch64-poky-linux-g++ -o linux_imx8_aarch64 --sysroot=/opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/ -I /opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/usr/include/ -L/opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/usr/lib event.cpp -Wall
cp linux_imx8_aarch64 /srv/nfsroot/integration_test/event_test
${TOOLCHAIN_armhf}/arm-linux-gnueabihf-g++ -o linux_imx6_armhf -I /opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/include/ -L /opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/lib event.cpp -Wall
cp linux_imx6_armhf /srv/nfsroot/integration_test/event_test
${TOOLCHAIN_wayland}/aarch64-poky-linux-g++ -o linux_wayland_aarch64 --sysroot=/opt/sdk/poky/2.1.2/sysroots/aarch64-poky-linux -I /opt/sdk/poky/2.1.2/sysroots/aarch64-poky-linux/usr/include -L /opt/sdk/poky/2.1.2/sysroots/aarch64-poky-linux/usr/lib event.cpp -Wall
cp linux_wayland_aarch64 /srv/nfsroot/integration_test/event_test
