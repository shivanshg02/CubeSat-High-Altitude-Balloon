import serial
from adafruit_rockblock import RockBlock
import time

uart = serial.Serial("/dev/ttyUSB0", 19200)
rb = RockBlock(uart)

rb.text_out = "first"
status = rb.satellite_transfer()

while True:
    f = open("/home/uno/Desktop/file.txt","r")
    line = f.readlines()[-1].strip()
    rb.text_out = line
    status = rb.satellite_transfer()
    if status[0] < 8:
        print("Successful transmission:")
        print(line)
    else:
        print("Trasmission failed, aw")
        print(line)
    time.sleep(10)
#wooo it's working
