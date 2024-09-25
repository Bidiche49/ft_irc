#!/bin/bash

SERVER="localhost"
PORT="6667"

PASS="PASS go"
NICK="NICK alex"
USER="USER alex 0 * :Alexandru Mihai Costin"

# Send commands to the IRC server
{
    echo "$PASS"
    echo "$NICK"
    echo "$USER";
	cat
} | nc -i 1 $SERVER $PORT
