# raspberry pi code for RFM9x - receiver end
# just listens for packets and prints them out
#
import time
import board
import busio
import digitalio
import adafruit_rfm9x
import adafruit_ds1307
import picamera

# Define radio parameters.
RADIO_FREQ_MHZ = 420.69   # Frequency of the radio in Mhz. Must match your
# module! Can be a value like 915.0, 433.0, etc.

# Define pins connected to the chip.
CS = digitalio.DigitalInOut(board.CE1)
RESET = digitalio.DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, RADIO_FREQ_MHZ)
i2c = board.I2C()
rtc = adafruit_ds1307.DS1307(i2c)


rfm9x.enable_crc = True
rfm9x.tx_power = 23
rfm9x.coding_rate = 7
rfm9x.signal_bandwidth = 62500
rfm9x.spreading_factor = 9
rfm9x.preamble_length = 8

rfm9x.node = 1
rfm9x.destination = 2
rfm9x.identifier = 0x69

def get_packet():
    t = rtc.datetime()


while True:
    rfm9x.send(bytes("Hello world!\r\n", "utf-8"))
    packet = rfm9x.receive(with_header=True,with_ack=False,timeout=1999)
    if rfm9x.crc_error():
        print("CRC error!")
    if packet is not None:
        # Received a packet!
        # Print out the raw bytes of the packet:
        print([hex(x) for x in packet[0:4]], "  ||  ", packet[4:].decode(), "  ||  SNR: {0}dB".format(rfm9x.last_snr))

