#!/bin/sh

echo "Running fpp-tasmota PreStart Script"

BASEDIR=$(dirname $0)
cd $BASEDIR
cd ..
make
