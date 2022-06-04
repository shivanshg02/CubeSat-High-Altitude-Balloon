import serial
from adafruit_rockblock import RockBlock
import time

uart = serial.Serial("/dev/ttyUSB0", 19200)
rb = RockBlock(uart)

rb.text_out = "first"
status = rb.satellite_transfer()

while True:
    f = open("/home/target_file")
    line = f.readlines()[-1].strip()
    status = rb.satellite_transfer()
    if status[0] < 8:
        print("Successful transmission:")
        print(line)
    else:
        print("Trasmission failed, aw")
    time.sleep(20)
#wooo it's working
