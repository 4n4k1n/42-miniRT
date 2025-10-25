#!/bin/bash
git clone https://github.com/4n4k1n/42-miniRT.git ~/miniRT

if [ $# -eq 0 ]; then
    read -p "Enter IP address: " ip_address
else
    ip_address=127.0.0.1
fi

make -C ~/miniRT
~/miniRT/miniRT --worker $ip_address
rm -rf ~/miniRT