#!/bin/sh -e
cmake . --build bin
make -C bin clean
make -C bin
sudo nrfjprog --sectorerase --program bin/receiver.hex
sudo nrfjprog --reset
