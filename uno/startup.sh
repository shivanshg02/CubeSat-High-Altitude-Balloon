#!/bin/bash

/bin/sleep 30

cd /home/uno/Desktop/CubeSat-High-Altitude-Balloon/

python3 /home/uno/Desktop/CubeSat-High-Altitude-Balloon/uno/logger.py & 
python3 /home/uno/Desktop/CubeSat-High-Altitude-Balloon/uno/sender.py & 
python3 /home/uno/Desktop/CubeSat-High-Altitude-Balloon/uno/recordvideo.py

/bin/sleep 60

sudo reboot