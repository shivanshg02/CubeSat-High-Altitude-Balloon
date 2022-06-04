#!/bin/bash

/bin/sleep 30

cd /home/uno/Desktop/CubeSat-High-Altitude-Balloon/uno

python3 logger.py & 
python3 sender.py & 
python3 recordvideo.py

/bin/sleep 60

sudo reboot