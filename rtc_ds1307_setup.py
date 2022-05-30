import time
import board
import adafruit_ds1307

i2c = board.I2C()  # uses board.SCL and board.SDA
rtc = adafruit_ds1307.DS1307(i2c)
t = time.struct_time((2022, 5, 31, 23, 59, 59, 1, -1, -1))
rtc.datetime = t