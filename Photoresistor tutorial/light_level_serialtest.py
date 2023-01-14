##
## This script is dependent on pyserial which is installed separately from python
##
import serial
import time

arduino = serial.Serial(port = 'COM3', timeout=0) #set up object to connect to arduino over COM3
time.sleep(2)

var = ""        # Initialize variables with the correct type before using in main loop
buffer = b''    #
endByte = b'\r' #

# Output a label before moving into main loop where data is overwritten with a carriage return
print("The current light level is: ")

while True:
    if arduino.in_waiting >0: # Check if there is data to be read from serial port
        while buffer != endByte:                # Loop to continue reading one byte at a time until reaching \r
            buffer = arduino.read()             #
            var += buffer.decode('ascii')       # Convert ascii from arduino into utf-8 for python3
        if var != "":               # Check if there is data in var to be output
            print('\r     \r',var, end="") # Output var
            var = ""                # Reset var and buffer
            buffer = b''            #
            
