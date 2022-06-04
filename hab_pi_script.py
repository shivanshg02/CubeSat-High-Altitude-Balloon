#!/usr/bin/python3

import time
import board
import busio
import digitalio
import pigpio
import adafruit_rfm9x
import picamera
import serial
import threading
import datetime as dt

START_TIME = time.time()

# Pi Camera
camera = picamera.PiCamera()
camera.resolution = (1920,1080)
camera.framerate = 30
camera.awb_mode = 'auto'
camera.video_stabilization = True
camera.annotate_background = picamera.Color('black')
camera.annotate_text = dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
camera_record_thread = None
STOP_FLAG = False
def video_loop():
    global STOP_FLAG
    while not STOP_FLAG:
        start = dt.datetime.now()
        camera.start_recording("/home/dos/Desktop/videos/"+dt.datetime.now().strftime('%Y%m%d_%H%M%S')+".h264")
        while (dt.datetime.now() - start).seconds < 5*60 and not STOP_FLAG:
            camera.annotate_text = dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            camera.wait_recording(0.2)
        camera.stop_recording()
def start_video():
    global camera_record_thread
    camera_record_thread = threading.Thread(target=video_loop)
    camera_record_thread.start()
def stop_video():
    global camera_record_thread,STOP_FLAG
    STOP_FLAG = True
    if camera_record_thread is not None:
        camera_record_thread.join()
    camera_record_thread = None



# I2C comms with arduino
I2C_ADDR=9
pi = pigpio.pi()
if not pi.connected:
    exit()
pi.bsc_i2c(I2C_ADDR)
current_data = b""
def i2c(id, tick):
    global pi, current_data, packet_ready

    s, b, d = pi.bsc_i2c(I2C_ADDR)

    if b != bytearray(b'\n'):
        current_data += bytes(d)
    else:
        send_arduino_packet()
        current_data = b""

e = pi.event_callback(pigpio.EVENT_BSC, i2c)



# Seeed board for 433.4 MHz beacon
pi.set_mode(16,pigpio.OUTPUT)
pi.write(16,1)
ser = serial.Serial('/dev/ttyS0', 9600)



# Define radio parameters.
RADIO_FREQ_MHZ = 420.69 
CS = digitalio.DigitalInOut(board.CE1)
RESET = digitalio.DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, RADIO_FREQ_MHZ)
rfm9x.enable_crc = True
rfm9x.tx_power = 23
rfm9x.coding_rate = 9
rfm9x.signal_bandwidth = 62500
rfm9x.spreading_factor = 9
rfm9x.preamble_length = 8



def send_arduino_packet():
    rfm9x.send(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S').encode() + b" || " + bytes(current_data))

def health_signal():
    rfm9x.send(dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S').encode()+b" i'm still alive")

def seeed_homing_signal():
    ser.write(b"aaa")

#############################################################################

start_video()
while True:
    try:
        seeed_homing_signal()
        time.sleep(5)
    except:
        e.cancel()
        pi.stop()
        stop_video()
        exit()

