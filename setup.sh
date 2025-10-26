#!/bin/bash

if [ -d ~/miniRT ]; then
    echo "Repository already exists at ~/miniRT_apregitz"
else
    git clone https://github.com/4n4k1n/42-miniRT.git ~/miniRT_apregitz
fi

if [ -z "$IP_ADDRESS" ]; then
    read -p "Enter IP address: " ip_address
    export IP_ADDRESS="$ip_address"
else
    echo "Using IP_ADDRESS from environment: $IP_ADDRESS"
    ip_address="$IP_ADDRESS"
fi

cd ~/miniRT_apregitz
# git checkout 60-multi-server-rendering
make
~/miniRT_apregitz/miniRT --worker $ip_address