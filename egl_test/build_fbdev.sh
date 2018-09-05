export TOOLCHAIN_aarch64=/opt/sdk/imx8dvsabreauto_wayland/x86_64-linux/usr/bin/aarch64-poky-linux
export TOOLCHAIN_armhf=/opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin
${TOOLCHAIN_aarch64}/aarch64-poky-linux-g++ -o fbdev_test_imx8 --sysroot=/opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/ -DLINUX -DEGL_API_FB -I /opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/usr/include/ -L/opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/usr/lib test.cpp -lGLESv2 -lEGL -lGAL -lm -ldl -Wall
cp fbdev_test_imx8 /srv/nfsroot/integration_test/fb_test
${TOOLCHAIN_armhf}/arm-linux-gnueabihf-g++ -o fbdev_test_imx6 -DLINUX -DEGL_API_FB -I /opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/include/ -L /opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/lib test.cpp -lGLESv2 -lEGL -lGAL -lm -ldl -Wall
cp fbdev_test_imx6 /srv/nfsroot/integration_test/fb_test
