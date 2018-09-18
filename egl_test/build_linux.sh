export TOOLCHAIN_aarch64=/opt/sdk/imx8dvsabreauto_wayland/x86_64-linux/usr/bin/aarch64-poky-linux
export TOOLCHAIN_armhf=/opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf_clone/bin
${TOOLCHAIN_aarch64}/aarch64-poky-linux-g++ -o linux_imx8_aarch64 --sysroot=/opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/ -DLINUX -DEGL_API_FB -I /opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/usr/include/ -L/opt/sdk/imx8dvsabreauto_wayland/imx8dvsabreauto/usr/lib test.cpp -lGLESv2 -lEGL -lGAL -lm -ldl -Wall
cp linux_imx8_aarch64 /srv/nfsroot/integration_test/egl_test
${TOOLCHAIN_armhf}/arm-linux-gnueabihf-g++ -o linux_imx6_armhf -DLINUX -DEGL_API_FB -I /opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf_clone/arm-linux-gnueabihf/include/ -L /opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf_clone/arm-linux-gnueabihf/lib -Wl,-rpath=/opt/sdk/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf_clone/arm-linux-gnueabihf/lib test.cpp -lGLESv2 -lEGL -lGAL -lm -ldl -Wall
cp linux_imx6_armhf /srv/nfsroot/integration_test/egl_test
