#!/bin/sh

echo "Running fpp-plugin-data2curl PreStart Script"

BASEDIR=$(dirname $0)
cd $BASEDIR
cd ..
make
