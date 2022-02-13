#!/bin/sh

# START SERVER OPTIONS
export PORT=8081
export MAX_CONNECTIONS=16
export TIMEOUT=120
export MEMORY_LIMIT=65536
export HTTPS_MEM_KEY_PATH=~/certs/server_ca/private/smoothstack_server.key
export HTTPS_MEM_CERT_PATH=~/certs/server_ca/certs/smoothstack_server.crt
# END SERVER OPTIONS

if [ -f ./build/server ];
then
    ./build/server
else
    echo "./build/server not found"
    exit
fi