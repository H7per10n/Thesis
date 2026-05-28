#!/bin/bash
# send.sh - Cross-compile and send to SBC

if [ -z "$1" ]; then
    echo "Usage: $0 <sbc_ip>"
    echo "Example: $0 192.168.123.100"
    exit 1
fi

SBC=$1
executable="simple_test"

echo "Cross-compiling for ARM..."
arm-linux-gnueabihf-gcc -Wall -O2 -static -o ${executable} simple_test.c can_if.c -lm -lpthread|| exit 1

strip ${executable}

echo "Sending to root@$SBC:/tmp/${executable}"
scp ${executable} root@$SBC:/tmp/

ssh root@$SBC "chmod +x /tmp/${executable}"

echo ""
echo "Done. SSH and run:"
echo "  ssh root@$SBC"
echo "  /tmp/${executable} can0 20"
