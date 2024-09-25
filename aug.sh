#!/bin/bash

SERVER="localhost"
PORT="6667"

PASS="PASS go"
NICK="NICK aug"
USER="USER aug 0 * :Augustin Drye"

# Send commands to the IRC server
{
    echo "$PASS"
    echo "$NICK"
    echo "$USER"; cat
} | nc -i 1 $SERVER $PORT
