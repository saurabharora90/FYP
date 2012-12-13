#!/bin/sh

#
# Initialization script for Arduino stuff
#
# Loads modules and launches proxy server application
# required for communication with Arduino module.
#

echo Enabling USB, powering up Arduino and launching AT Commands Proxy

echo Loading usbserial.ko
insmod /data/video/ardudrone/usbserial.ko

echo Loading ftdi_sio.ko
insmod /data/video/ardudrone/ftdi_sio.ko

echo Enabling USB Port
gpio 127 -d ho 1
gpio 127 -d i

echo Loading dwc_otg.ko
insmod /data/video/ardudrone/dwc_otg.ko

echo Waiting 3s for the device to be ready
sleep 3

echo Setting correct baud rate of 115200 for /dev/ttyUSB0
stty -F /dev/ttyUSB0 115200

echo Launching AT Command Proxy server
/data/video/ardudrone/atcomproxy_arm &
