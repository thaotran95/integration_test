export TOOLCHAIN_qnx660=/opt/sdk/qnx660/host/linux/x86/usr/bin
${TOOLCHAIN_qnx660}/QCC -DEGL_OPENGL_ES_API -o qnx660_test -I /opt/sdk/qnx660/target/qnx6/usr/include/ -L /opt/sdk/qnx660/target/qnx6/armle-v7/usr/lib test_qnx660.cpp -lGLESv2 -lEGL -lm -lscreen -Wall -ggdb
cp qnx660_test /srv/nfsroot/integration_test/egl_test

