import picamera
from time import sleep
camera = picamera.PiCamera()
camera.start_preview()
camera.start_recording(‘recorded.h264’)
camera.wait_recording(10)
camera.stop_recording()
camera.stop_preview()