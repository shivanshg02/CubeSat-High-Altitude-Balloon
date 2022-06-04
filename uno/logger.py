#!/usr/bin/env python3
import serial
import time
import board
from adafruit_rockblock import RockBlock


def cleanser(string):
    ret_str = ""
    for i in string:
        if i > 32 and i < 127:
            ret_str += i
    return ret_str

def add_timestamp(string):
    

arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1000)
arduino.reset_input_buffer()

target_file = open("file.txt")

while True:
    if arduino.in_waiting > 0:
        line = arduino.readline()
        if len(line) > 3:
            line = cleanser(line).decode('utf-8').rstrip()
            target_file.write("\n"+line)
            target_file.flush()
#WOOO IT'S WORKING

