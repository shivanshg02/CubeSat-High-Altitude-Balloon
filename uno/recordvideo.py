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

try:
    start_video()
except:
    stop_video()