#!/bin/bash

/bin/sleep 30

sudo hwclock -s

cd /home/dos/Desktop/CubeSat-High-Altitude-Balloon/

sudo python3 hab_pi_script.py
