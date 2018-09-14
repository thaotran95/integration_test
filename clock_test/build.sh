export TOOLCHAIN_aarch64=/opt/sdk/imx8dvsabreauto_wayland/x86_64-linux/usr/bin/aarch64-poky-linux
export TOOLCHAIN_armhf=/opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin
export TOOLCHAIN_qnx660=/opt/sdk/qnx660/host/linux/x86/usr/bin/
export TOOLCHAIN_wayland=/opt/sdk/poky/2.1.2/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux
export TOOLCHAIN_qnx700=/opt/sdk/qnx700/host/linux/x86_64/usr/bin
${TOOLCHAIN_aarch64}/aarch64-poky-linux-g++ -o linux_imx8_aarch64 --sysroot=/opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/ -I /opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/usr/include/ -L/opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/usr/lib clock.cpp -lrt -Wall
cp linux_imx8_aarch64 /srv/nfsroot/integration_test/clock_test
${TOOLCHAIN_armhf}/arm-linux-gnueabihf-g++ -o linux_imx6_armhf -I /opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/include/ -L /opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/lib clock.cpp -lrt -Wall
cp linux_imx6_armhf /srv/nfsroot/integration_test/clock_test 
source /opt/sdk/qnx660/qnx660-env.sh
${TOOLCHAIN_qnx660}/QCC -o qnx660_screen_arm -I /opt/sdk/qnx660/target/qnx6/usr/include -L /opt/sdk/qnx660/target/qnx6/usr/lib clock.cpp -Wall
cp qnx660_screen_arm /srv/nfsroot/integration_test/clock_test
${TOOLCHAIN_wayland}/aarch64-poky-linux-g++ -o linux_wayland_aarch64 --sysroot=/opt/sdk/poky/2.1.2/sysroots/aarch64-poky-linux -L /opt/sdk/poky/2.1.2/sysroots/x86_64-pokysdk-linux/usr/lib clock.cpp -lrt -Wall
cp linux_wayland_aarch64 /srv/nfsroot/integration_test/clock_test
source /opt/sdk/qnx700/qnx700-env.sh
${TOOLCHAIN_qnx700}/qcc -o qnx700_screen_aarch64 -I /opt/sdk/qnx700/target/qnx7/aarch64le/usr/include -L /opt/sdk/qnx700/target/qnx7/aarch64le/usr/lib clock.cpp -Wall
cp qnx700_screen_aarch64 /srv/nfsroot/integration_test/clock_test
