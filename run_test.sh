#!/bin/sh

# START SERVER OPTIONS

# leaving empty for default values to kick in

# END SERVER OPTIONS

if [ -f ./build/test/test ];
then
    ./build/test/test
else
    echo "./build/test/test not found"
    exit
fi