#!/bin/bash

if [ -d ~/miniRT ]; then
    echo "Repository already exists at ~/miniRT"
else
    git clone https://github.com/4n4k1n/42-miniRT.git ~/miniRT
fi

if [ -z "$IP_ADDRESS" ]; then
    read -p "Enter IP address: " ip_address
    export IP_ADDRESS="$ip_address"
else
    echo "Using IP_ADDRESS from environment: $IP_ADDRESS"
    ip_address="$IP_ADDRESS"
fi

cd ~/miniRT
git checkout 60-multi-server-rendering
make
~/miniRT/miniRT --worker $ip_address