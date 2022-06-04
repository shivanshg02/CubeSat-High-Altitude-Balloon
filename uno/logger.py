#!/usr/bin/env python3
import serial
import time
import board
from adafruit_rockblock import RockBlock
from datetime import datetime

def cleanser(string):
    ret_str = ""
    for i in string:
        if i > 32 and i < 127:
            ret_str += chr(i)
    return ret_str


def add_timestamp(string):
    now = datetime.now()
    date_time = now.strftime("%m/%d/%Y,%H:%M:%S;")
    return date_time+string

arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1000)
arduino.reset_input_buffer()

target_file = open("/home/uno/Desktop/file.txt",'w')

while True:
    if arduino.in_waiting > 0:
        line = arduino.readline()
        if len(line) > 3:
            line = add_timestamp(cleanser(line).rstrip())
            target_file.write("\n"+line)
            target_file.flush()
#WOOO IT'S WORKING

