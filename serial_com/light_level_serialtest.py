import serial
import time

arduino = serial.Serial(port = 'COM3', timeout=0)
time.sleep(2)

var = ""
i=0
buffer = b''
endByte = b'\r'
print("The current light level is: ")
while True:
    if arduino.in_waiting >0:
        while buffer != endByte:
            buffer = arduino.read()
            var += buffer.decode('ascii')
        if var != "":
            print('\r',var, end="")
            var = ""
            buffer = b''
            
