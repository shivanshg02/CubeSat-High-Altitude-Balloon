#!/bin/bash

sudo pigpiod

cd /home/dos/Desktop/CubeSat-High-Altitude-Balloon/

git pull

sudo python3 hab_pi_script.py
