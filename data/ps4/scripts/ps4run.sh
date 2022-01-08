#!/bin/bash
curl -T /home/cpasjuste/dev/multi/libcross2d/cmake-build-debug-ps4/eboot.bin ftp://192.168.0.189:2121/data/eboot.bin
tio /dev/ttyUSB0
