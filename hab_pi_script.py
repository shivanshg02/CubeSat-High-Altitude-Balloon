# raspberry pi code for RFM9x - receiver end
# just listens for packets and prints them out
#
import time
import board
import busio
import digitalio
import pigpio
import adafruit_rfm9x
import picamera

I2C_ADDR=9

# Define radio parameters.
RADIO_FREQ_MHZ = 420.69   # Frequency of the radio in Mhz. Must match your
# module! Can be a value like 915.0, 433.0, etc.

pi = pigpio.pi()
if not pi.connected:
    exit()
pi.bsc_i2c(I2C_ADDR)

# Define pins connected to the chip.
CS = digitalio.DigitalInOut(board.CE1)
RESET = digitalio.DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, RADIO_FREQ_MHZ)


rfm9x.enable_crc = True
rfm9x.tx_power = 23
rfm9x.coding_rate = 9
rfm9x.signal_bandwidth = 62500
rfm9x.spreading_factor = 7
rfm9x.preamble_length = 8

current_data = 'aaa'
def i2c(id, tick):
   global pi, current_data

   s, b, d = pi.bsc_i2c(I2C_ADDR)

   if b:
    current_data = d
     
e = pi.event_callback(pigpio.EVENT_BSC, i2c)



def main():
    rfm9x.send(bytes(current_data,"utf-8"))
    
    # packet = rfm9x.receive(with_header=True,with_ack=False,timeout=1999)
    # if packet is not None:
    #     pass\
    time.sleep(3)


while True:
    try:
        main()
    except:
        e.cancel()
        pi.stop()

