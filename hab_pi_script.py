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

# Pi Camera
camera = picamera.PiCamera()
camera.resolution = (1920,1080)
camera.framerate = 30
camera.awb_mode = 'auto'
camera.video_stabilization = True
camera.annotate_background = picamera.Color('black')
camera.annotate_text = dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
camera_record_thread = None
def video_loop():
    camera.start_recording("/home/dos/Desktop/videos/"+str(round(time.time(),0))+".h264")
    camera.wait_recording(5*60)
    camera.stop_recording()
def start_video():
    global camera_record_thread
    camera_record_thread = threading.Thread(target=video_loop)
    camera_record_thread.start()
def stop_video():
    global camera_record_thread
    camera.stop_recording()
    if camera_record_thread is not None:
        camera_record_thread.join()
    camera_record_thread = None



# I2C comms with arduino
I2C_ADDR=9
pi = pigpio.pi()
if not pi.connected:
    exit()
pi.bsc_i2c(I2C_ADDR)
current_data = bytes('BEGIN',"utf-8")
def i2c(id, tick):
   global pi, current_data

   s, b, d = pi.bsc_i2c(I2C_ADDR)

   if b:
    current_data = d
e = pi.event_callback(pigpio.EVENT_BSC, i2c)



# Seeed board for 433.4 MHz beacon
pi.set_mode(16,pigpio.OUTPUT)
pi.write(16,0)
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



def main():
    rfm9x.send(bytes(current_data))
    # packet = rfm9x.receive(with_header=True,with_ack=False,timeout=1999)
    # if packet is not None:
    #     pass

def seeed_homing_signal():
    ser.write(b"aaa")

#############################################################################

start_video()
while True:
    try:
        main()
        time.sleep(1)
        seeed_homing_signal()
        time.sleep(1)
    except:
        e.cancel()
        pi.stop()
        stop_video()
        exit()

