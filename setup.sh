#!/bin/bash
git clone https://github.com/4n4k1n/42-miniRT.git ~/miniRT

if [ $# -eq 0 ]; then
    read -p "Enter IP address: " ip_address
else
    rm -rf ~/miniRT
    exit 1
fi

cd ~/miniRT
git checkout 60-multi-server-rendering
make
~/miniRT/miniRT --worker $ip_address
rm -rf ~/miniRT