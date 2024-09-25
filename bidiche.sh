#!/bin/bash

SERVER="localhost"
PORT="6667"

PASS="PASS go"
NICK="NICK bidiche"
USER="USER bidiche 0 * :Nicolazic Tardy"

# Send commands to the IRC server
{
    echo "$PASS"
    echo "$NICK"
    echo "$USER"; cat
} | nc -i 1 $SERVER $PORT
