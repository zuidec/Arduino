#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>

#define ERROR_HANDLE_EOF 38

void FlushBuffer(char*, int);
char* COMPortName = "COM3";
DCB port;

int main()
{
	HANDLE arduino; // Create new filestream named arduino
	DWORD comError; // DWORD to use with GetLastError()
	
	
	try 
	{
		std::cout << "Attempting to connect to " << COMPortName << "..." << std::endl; 	// Attempt to connect to arduino
		arduino = CreateFileA (COMPortName,			// COM port 3
				GENERIC_READ | GENERIC_WRITE,	// Open read and write
				0,
				NULL,
				OPEN_EXISTING,					// Opens existing COM port
				FILE_FLAG_OVERLAPPED,			// Was originally FILE_FLAG_OVERLAPPED, but using 0 prevents error 87 
				NULL);						

		if(arduino == INVALID_HANDLE_VALUE) throw 'a'; // throw exception if handle is invalid
		
		std::cout << "Successfully connected on "<< COMPortName <<", attempting to configure port settings and timeouts...  " << std::endl;
		
		
		COMMTIMEOUTS arduinoTimeouts;					// Create a timeout profile with the following parameters

		arduinoTimeouts.ReadIntervalTimeout = 1;
		arduinoTimeouts.ReadTotalTimeoutConstant = 1;
		arduinoTimeouts.ReadTotalTimeoutMultiplier = 1;
		arduinoTimeouts.WriteTotalTimeoutConstant = 1;
		arduinoTimeouts.WriteTotalTimeoutMultiplier = 1;

		
		
		

		memset(&port, 0, sizeof(port));
		port.DCBlength = sizeof(port);
		if (!GetCommState(arduino, &port)) throw 'c';
		if (!BuildCommDCB("baud=9600 parity=n data=8 stop=1", &port)) throw 'd'; // Setup DBC port correctly
		if (!SetCommState(arduino, &port)) throw 'e';
		std::cout << "Settings successfully adjusted...  " << std::endl;
		if (!SetCommTimeouts(arduino, &arduinoTimeouts)) throw 'b'; // Attempt to set timeout profile for arduino and throw exception if unsuccessful
		std::cout << "Timeouts successfully configured!" << std::endl;

		Sleep(500);

	}
	catch (char a)
	{
		comError = GetLastError();
		
		// Output correct error message with last error
		if(a=='a') std::cout << "Error connecting on " << COMPortName << ", exiting now, error number: "<< comError << std:: endl;
		else if(a=='b') std::cout << "Error setting timeouts for " << COMPortName << ", exiting now, error number: " << comError << std::endl;
		else if (a == 'c') std::cout << "Error getting comm state for " << COMPortName << ", exiting now, error number: " << comError << std::endl;
		else if (a == 'd') std::cout << "Error building comm DCB for " << COMPortName << ", exiting now, error number: " << comError << std::endl;
		else if (a == 'e') std::cout << "Error adjusting port setting for " << COMPortName << ", exiting now, error number: " << comError << std::endl;
		
		CloseHandle(arduino);		// Close handle and exit																			
		return -1;									 
	}
	catch (...)
	{
		comError = GetLastError();
		std::cout << "Other unknown error occurred, exiting now, error number: " << comError << std::endl;	// Catch exception and exit if
		CloseHandle(arduino);																								// unsuccessful
		return -1;
	}
	
	char buffer[32];
	FlushBuffer(&buffer[0], sizeof(buffer)); // Send the buffer to be initialized to \0 in all positions

	std::cout << std::endl << "Attempting to read data from " << COMPortName << "..." << std::endl << std::endl;
	
	OVERLAPPED arduino_OVERLAP;

	arduino_OVERLAP.hEvent = NULL;
	arduino_OVERLAP.Offset = 0;
	arduino_OVERLAP.OffsetHigh = 0;

	BOOL EndOfFile = FALSE;

	while (TRUE) /// Enter into main loop for receiving data from arduino
	{
		// Attempt to read a line from arduino and store into the buffer
		try
		{
			//std::cout << "In try block";
			for (int i = 0; !EndOfFile; i++) // Set up loop to read until \r is found
			{
				
				int returnValue; // Set up a variable to store the return value of ReadFile() function to check for a read error and throw exception
				DWORD bytesRead; 
				
				
				
				returnValue = ReadFile(arduino, &buffer[i], 1, &bytesRead, &arduino_OVERLAP); // Read one byte at a time from arduino and store in buffer
				//if (bytesRead == 0);
					
				//if (buffer[i] == '\r') buffer[i] = '\0';
				//std::cout << buffer[i];

				if (returnValue == 0 && GetLastError()== ERROR_HANDLE_EOF) throw; // ReadFile() only returns 0 if it is unsuccessful
				
				if (buffer[i] == '\0') EndOfFile = true;
			}
			if (buffer[0] != '\0')
			{
				std::cout << "\r                                                           \rThe current light level is: " << buffer;
				FlushBuffer(&buffer[0], sizeof(buffer)); // Send the buffer to be set back to \0 in all positions
			}
			EndOfFile = false;
		}
		catch (...)
		{
			 comError = GetLastError();
			std::cout << "Error reading from " << COMPortName << ", exiting with error number " << comError << std::endl; // Output last error and exit 
			CloseHandle(arduino);
			return -1;
		}
		Sleep(200);
	}

	CloseHandle(arduino);
	return 0;
}

void FlushBuffer(char* buf, int buf_size)
{
	for (int i = 0; i < buf_size;i++)
	{
		buf[i] = '\0';			// Reset all members of buffer to \0
	}

	return;
}
