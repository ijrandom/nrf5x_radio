#!/bin/sh -e
cmake . --build bin
make -C bin clean
make -C bin
sudo nrfjprog --sectorerase --program bin/transmitter.hex
sudo nrfjprog --reset
