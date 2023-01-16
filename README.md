# Arduino README.md

## /photoresistor_tutorial/
### Basic programs to read a photoresistor value from arduino and output on the command line.

- light_level_serialtest.py is a Python 3 script to read photoresistor data from the arduino on your PC.

- .ignore.light_level_serialtest.cpp is a C++ source code to read photoresistor data from the arduino on your PC. 
	Currently this will only work on Windows due to use of #include <windows.h>. 
	Unusual naming convention is to prevent conflict with the Arduino IDE.

- light_level_serialtest.png is a schematic on how to set up arduino to work with the photoresistor.

- Photoresistor tutorial.ino is an Arduino IDE sketch for arduino to read a photoresistor and output data to the computer.

## /ArduinoCOM/
### ArduinoCOM is a class being developed to ease the use of serial communication between Windows and Arduino

***ArduinoCOM is dependent on windows.h and will not function on Unix systems as is.***

- ArduinoCOM.h and ArduinoCOM.cpp contain the following public members of the ArduinoCOM class:
	
	- ArduinoCOM::Connect(char\* COMPortName, int BaudRate) : Connects to the Arduino on the specified port at the specified baud rate.
	
	- ArduinoCOM::Disconnect() : Disconnects from the Arduino and cleans up.
	
	- ArduinoCOM::GetError() : Gets last internal error reported by the ArduinoCOM class. *Does not report errors not explicitly defined in ArduinoCOM*
	
	- ArduinoCOM::ClearErrors() : Clears internal errors in the ArduinoCOM class;
	
	- ArduinoCOM::Read() : Read one byte from Arduino.
		
	- ArduinoCOM::ReadLine() : Read a null terminated "\0" line from Arduino.
	
	- ArduinoCOM::Write() : Write one byte to Arduino.
	
	- ArduinoCOM::WriteLine() : Write a null terminated "\0" line to Arduino.
	
	- ArduinoCOM::FlushBuffer() : Clear ArduinoCOM's internal input buffer.