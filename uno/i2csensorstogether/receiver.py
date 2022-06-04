import time

# CircuitPython / Blinka
'''
import board

uart = board.UART()
uart.baudrate = 19200
'''
# via USB cable
from picamera import PiCamera
import serial
uart = serial.Serial("/dev/ttyUSB0", 19200)

from adafruit_rockblock import RockBlock

rb = RockBlock(uart)


def receive(rb):
    status = rb.satellite_transfer()
    # loop as needed
    retry = 0
    while status[0] > 8:
        print("Establishing comms...")
        time.sleep(10)
        status = rb.satellite_transfer()
        print(retry, status)
        retry += 1
    print("\nDONE.")

    return rb.text_in

camera = PiCamera()
camera.resolution = (1280, 720)
time.sleep(2)
count = 0
while True:
    time.sleep(10)
    text = receive(rb)
    if text == 'take_pic':
        print("Taking picture...")
        camera.capture("~/Desktop/img{}.jpg".format(str(count)))
    else:
        print(text)