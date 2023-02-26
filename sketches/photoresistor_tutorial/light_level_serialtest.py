##
## This script is dependent on pyserial which is installed separately from python
##
import serial
import time

arduino = serial.Serial(port = 'COM12', timeout=0) #set up object to connect to arduino over COM3
time.sleep(2)

var = b''        # Initialize variables with the correct type before using in main loop
buffer = b''    #
endByte = b'\x03' #
print(endByte)
# Output a label before moving into main loop where data is overwritten with a carriage return
#print("The current light level is: ")

while True:
    if arduino.in_waiting >0: # Check if there is data to be read from serial port
        print("Bytes remaining in buffer from Arduino: " + str(arduino.in_waiting))
        while buffer != endByte:                # Loop to continue reading one byte at a time until reaching \r
            buffer = arduino.read()             #
            print(buffer.decode('ascii'),sep="",end="")
         #   var += buffer       # Convert ascii from arduino into utf-8 for python3
        if var != b'':               # Check if there is data in var to be output
 #           print('\r     \r',var, end="") # Output var
            print(var.decode('ascii'))
            var = b''                # Reset var and buffer
            buffer = b''            #
            
