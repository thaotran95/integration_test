*** Settings ***
Documentation          This example demonstrates executing commands on a remote machine
...                    and getting their output and the return code.
...
...                    Notice how connections are handled as part of the suite setup and
...                    teardown. This saves some time when executing several test cases.

Library                SSHLibrary
Library                OperatingSystem

#Suite Setup            Open Connection And Log In
#Suite Teardown         Close All Connections

*** Test Cases


Executing kzb_player
    [Documentation]    kzb_player running
    Enable Ssh Logging    ssh.log
	Write              cd /nfs/integration_test/fb_test
    ${kzb_player}=     Open Connection And Log In
    Run Keyword If  '${HOST}'=='192.168.0.30'  Write  export GRAPHICS_ROOT=/usr/lib/graphics/iMX6X
    Run Keyword If  '${HOST}'=='192.168.0.30'  Write  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
    Run Keyword If  '${HOST}'=='192.168.0.26'  Write  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/sdcard:/nfs/Release
    Run Keyword If  '${HOST}'=='192.168.0.17'  Write  export XDG_RUNTIME_DIR=/var/run/user/0
    Run Keyword If  '${HOST}'=='192.168.0.16'  Write              ./linux_imx6_armhf
    ${info}=            Read     delay=5
    Log To Console     \n${info}
    Close All Connections
	
*** Keywords
Open Connection And Log In
    Open Connection       ${HOST}
    Login                 ${USERNAME}            ${PASSWORD}

