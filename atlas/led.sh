#! /bin/bash

HERE=$(dirname $(readlink -f "$0"))
LED=$HERE/led

cd $LED
make run LED=$1

