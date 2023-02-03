#include "ArduinoCOM.h"
#include <iostream> // Using to debug 

ArduinoCOM::ArduinoCOM()
{
	BaudRate = 9600;
	COMPortName = "";

	FlushBuffer(&ReadBuffer[0], sizeof(ReadBuffer));	// Clear buffer before we start reading
	FlushBuffer(&WriteBuffer[0], sizeof(WriteBuffer));	// Clear buffer before we start writing

	SetError("");	// Start with an empty error
}

ArduinoCOM::~ArduinoCOM()
{
	Disconnect();
}

void ArduinoCOM::Initialize()
{
	// When using asynchronous IO, ReadFile() requires the last argument to be a pointer to an overlapped object, which is declared here. We are required to set hEvent, Offset, and OffsetHigh, but there are more properties
	PortOverlap.hEvent = NULL;		//
	PortOverlap.Offset = 0;			// Needed for asynchronous Read, not currently in use
	PortOverlap.OffsetHigh = 0;		//
	//

	// Create a timeout profile with the following parameters
	PortTimeouts.ReadIntervalTimeout = 500;			//	500ms between receiving bytes before timing out
	PortTimeouts.ReadTotalTimeoutConstant = 100;	//	100ms added to (ReadTotalTimeoutMultiplier * bytes requested) before timing out
	PortTimeouts.ReadTotalTimeoutMultiplier = 500;	//	
	PortTimeouts.WriteTotalTimeoutConstant = 500;	//
	PortTimeouts.WriteTotalTimeoutMultiplier = 500;	//
	
	// Set required properties of DCB object
	memset(&Port, 0, sizeof(Port));		//
	Port.DCBlength = sizeof(Port);		//

}

int ArduinoCOM::Connect()
{
	Initialize();
	CommDCBParameter = "baud=" + std::to_string(BaudRate) + " parity=n data=8 stop=1";	// Variable to store string containing baud, etc to build DCB with
	std::cout << COMPortName << std::endl;
	try 
	{
		SerialPort = CreateFileA (COMPortName,	// Set COM port to connect to
				GENERIC_READ | GENERIC_WRITE,	// Open read and write
				0,								
				NULL,
				OPEN_EXISTING,					// Opens existing COM port
				0,								// Use FILE_FLAG_OVERLAPPED for asynchronous or 0 for synchronous
				NULL);
		
		if(SerialPort == INVALID_HANDLE_VALUE) throw 'a';						// throw exception if handle is invalid	
		if (!GetCommState(SerialPort, &Port)) throw 'c';						// Gets information about SerialPort to store in DCB
		if (!BuildCommDCB(CommDCBParameter.c_str(), &Port)) throw 'd';			// Build DCB according to CommDCBParameter variable
		if (!SetCommState(SerialPort, &Port)) throw 'e';						// Attempt to set SerialPort according to build parameters from DCB
		if (!SetCommTimeouts(SerialPort, &PortTimeouts)) throw 'b';				// Attempt to set timeout profile for arduino and throw exception if unsuccessful
	}
	catch(char a)
	{
		switch(a)		// Catch exceptions and use SetError() to store the correct error
		{
			case 'a':
				SetError("INVALID_HANDLE_ERROR");
				break;
			case 'b':
				SetError("SET_COMM_TIMEOUT_ERROR");
				break;
			case 'c':
				SetError("GET_COMM_STATE_ERROR");
				break;
			case 'd':
				SetError("DCB_BUILD_ERROR");
				break;
			case 'e':
				SetError("SET_COMM_STATE_ERROR");
				break;
			default:
				SetError("WINDOWS_ERROR: " + GetLastError()); // Use GetLastError() to retrieve any other errors not defined
				break;
		}
		
		CloseHandle(SerialPort);		// Close handle and exit																			
		return 0;						
	}
	catch (...)
	{
		CloseHandle(SerialPort);		// Close handle and exit																			
		return 0;
	}
	return 1;
}

std::string ArduinoCOM::GetError()
{
	return LastError;
}

void ArduinoCOM::ClearErrors()
{
	LastError = "";
}

void ArduinoCOM::Read()
{
	
}

bool ArduinoCOM::ReadLine(std::string* Line) // Takes a string pointer as argument to store the line to be read
{
	bool EndOfFile = false;		// Create an EOF flag to use in this function
	try		
	{
		for (int i = 0; !EndOfFile&& i<sizeof(ReadBuffer); i++) // Set up loop to read until \n is found
		{
			if (!ReadFile(SerialPort, &ReadBuffer[i], 1, &bytesRead, /*&PortOverlap*/ NULL)) // Read one byte at a time from arduino and store in buffer, ReadFile() returns 0 if there is a failure. Use PortOverlap as last argument for asynchronous IO, NULL for synchronous IO
			{
				COMError = GetLastError();
				i--;
				switch (COMError)
				{
				case ERROR_IO_PENDING:		// We expect to occasionally have IO pending, so ignore
					break;
				case ERROR_HANDLE_EOF:		// We expect to occasionally read to the end of the buffer, so ignore
					break;
				default:					// For all other errors, we should throw an exception
					throw COMError;
					break;

				}

			}
			if (bytesRead < 1) i--;

			if (ReadBuffer[i] == '\n' && i > 0)
			{
				EndOfFile = true;				// Set EOF flag if terminating character is reached
				//std::cout << ReadBuffer;		// Used to debug buffer errors
				for (int j = 0; j <= i; j++)
				{
					*Line += ReadBuffer[j];		// Convert buffer into a string to be returned							
				}
				FlushBuffer(&ReadBuffer[0], sizeof(ReadBuffer));		// Send the buffer to be set back to \0 in all positions
			}
		}
		
		EndOfFile = false; // Reset EOF flag
	}
	catch (...) //Catch exceptions caused by ReadFile() and exit
	{
		SetError("READ_ERROR " + std::to_string(COMError)); // Output last error and exit 
		Disconnect();
		return false;
	}
	return true;
}

void ArduinoCOM::Write()
{
	
}

bool ArduinoCOM::WriteLine(std::string* Line)
{
	//std::cout << Line->size() << std::endl;	// Used for debugging function
	for (int i = 0; i< Line->size(); i++)
	{ 
		WriteBuffer[i] = Line->at(i);			// Write the data from the string pointer into local buffer
	}
	WriteBuffer[Line->size()] = '\n';			// Null terminate the buffer
	bytesToWrite = Line->size() +1;				// Set bytesToWrite equal to the size of Line +1 to accomodate for the addition of null terminator
	
	bytesWritten = 0;							// Reset the number of bytes written

	try
	{
		
		if (!WriteFile(SerialPort, &WriteBuffer[0], bytesToWrite, &bytesWritten, NULL))		// Attempt to write bytesToWrite number of bytes
		{
			COMError = GetLastError();
			
			switch (COMError)
			{
			case ERROR_IO_PENDING:		// We expect to occasionally have IO pending, so ignore
				break;
			case ERROR_HANDLE_EOF:		// We expect to occasionally read to the end of the buffer, so ignore
				break;
			default:					// For all other errors, we should throw an exception
				throw COMError;
				break;

			}
		}
		else if (bytesWritten != bytesToWrite)
		{
			throw 'a';
		}
		
	}
	catch (char a)
	{
		if (a == 'a')	// Catch exception caused by WriteFile() not writing all the data from the buffer and disconnect
		{
			SetError("WRITE_INCOMPLETE " + std::to_string(bytesToWrite - bytesWritten) + " BYTES NOT WRITTEN");
			Disconnect();
			return false;
		}
	}
	catch (...) //Catch all other exceptions caused and exit
	{
		SetError("WRITE_ERROR " + std::to_string(COMError)); // Output last error and disconnect
		Disconnect();
		return false;
	}
	FlushBuffer(&WriteBuffer[0], sizeof(WriteBuffer));		// Clear out buffer before exiting
	return true;
}

void ArduinoCOM::SetError(std::string NewError)
{
	LastError = NewError;
}

void ArduinoCOM::Disconnect()
{
	FlushBuffer(&ReadBuffer[0], sizeof(ReadBuffer));
	FlushBuffer(&WriteBuffer[0], sizeof(WriteBuffer));
	CloseHandle(SerialPort);
}

void ArduinoCOM::FlushBuffer(char *buffer, int bufferSize)
{
	for (int i = 0; i < bufferSize; i++)
	{
		buffer[i] = '\0';			// Reset all members of buffer to \0
	}

	return;
}

bool ArduinoCOM::isValidPort(int COMPortNumber)
{
	
	char testPort[12] = "\\\\.\\COM";
	snprintf(testPort,sizeof(testPort),"\\\\.\\COM%d", COMPortNumber);
	char* tempPort = COMPortName;
	//std::cout << "DEBUG: char* testPort = \'" << testPort << "\'" << std::endl;
	COMPortName = testPort;
	if (Connect()==0) {
		Disconnect();
		COMPortName = tempPort;
		return false;
	}
	else {
		Disconnect();
		COMPortName = tempPort;
		return true;
	}
	
}

bool ArduinoCOM::isConnected()
{
	if (SerialPort == INVALID_HANDLE_VALUE) return false;
	else return true;

}