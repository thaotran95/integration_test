*** Settings ***
Documentation          Integration test result
Library                SSHLibrary
Library                OperatingSystem

#Suite Setup            Open Connection And Log In
#Suite Teardown         Close All Connections

*** Test Cases


Run EGL Test
    [Documentation]    EGL test is running
    Enable Ssh Logging    ssh.log
    ${kzb_player}=     Open Connection And Log In
    Run Keyword If  '${HOST}'=='192.168.0.30'  Write  export GRAPHICS_ROOT=/usr/lib/graphics/iMX6X
    Run Keyword If  '${HOST}'=='192.168.0.30'  Write  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
    Run Keyword If  '${HOST}'=='192.168.0.26'  Write  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/sdcard:/nfs/Release
    Run Keyword If  '${HOST}'=='192.168.0.17'  Write  export XDG_RUNTIME_DIR=/var/run/user/0
    Write              cd /nfs/integration_test/egl_test
    Run Keyword If  '${HOST}'=='192.168.0.16'  Write              ./linux_imx6_armhf
    Run Keyword If  '${HOST}'=='192.168.0.30'  Write              ./qnx660_screen_arm
    ${EGL}=            Read     delay=5
    Log To Console     \n${EGL}


Run Clock Test
    [Documentation]    EGL test is running
    Enable Ssh Logging    ssh.log
    ${kzb_player}=     Open Connection And Log In
    Run Keyword If  '${HOST}'=='192.168.0.30'  Write  export GRAPHICS_ROOT=/usr/lib/graphics/iMX6X
    Run Keyword If  '${HOST}'=='192.168.0.30'  Write  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
    Run Keyword If  '${HOST}'=='192.168.0.26'  Write  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/sdcard:/nfs/Release
    Run Keyword If  '${HOST}'=='192.168.0.17'  Write  export XDG_RUNTIME_DIR=/var/run/user/0
    Write              cd /nfs/integration_test/clock_test
    Run Keyword If  '${HOST}'=='192.168.0.16'  Write              ./linux_imx6_armhf
    Run Keyword If  '${HOST}'=='192.168.0.30'  Write              ./qnx660_screen_arm

    ${clock}=            Read     delay=0.1
    Log To Console     \n${clock}
    Close All Connections
	
*** Keywords
Open Connection And Log In
    Open Connection       ${HOST}
    Login                 ${USERNAME}            ${PASSWORD}

