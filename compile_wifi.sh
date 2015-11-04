#!/bin/sh
arm-xilinx-linux-gnueabi-g++ -c ./libini.c
arm-xilinx-linux-gnueabi-g++ -c ./wifi.cpp
arm-xilinx-linux-gnueabi-g++ -o ./wifi_main ./wifi.o ./libini.o
arm-xilinx-linux-gnueabi-g++ ./wpa_maintain.cpp -o ./wpa_maintain
arm-xilinx-linux-gnueabi-g++ ./wifi_maintain.cpp -o ./wifi_maintain
