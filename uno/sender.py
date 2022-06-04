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
'''
def add_timestamp(string):
'''

arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1000)
arduino.reset_input_buffer()

uart = serial.Serial("/dev/ttyUSB0", 19200)
rb = RockBlock(uart)
    
    #uart = serial.Serial("/dev/ttyUSB0", 19200)
    #rb = RockBlock(uart)
rb.text_out = "first"
status = rb.satellite_transfer()

while True:
    if arduino.in_waiting > 0:
        print("hi")
        line = arduino.readline()
        if len(line) > 3:
            line = cleanser(line).decode('utf-8').rstrip()
            rb.text_out = line
            status = rb.satellite_transfer()
            if status[0] < 8:
                print("Successful transmission")
                print(line)
#WOOO IT'S WORKING

